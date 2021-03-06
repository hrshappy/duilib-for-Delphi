//*******************************************************************
//
//       作者：ying32
//       QQ  ：1444386932
//       E-mail：1444386932@qq.com
//       本单元由CppConvert工具自动生成于2015-11-28 16:34:01
//       版权所有 (C) 2015-2015 ying32 All Rights Reserved
//
//       注：不要使用文件直接替换了，现后期已经改为手动维护了
//*******************************************************************

#include "stdafx.h"
#include "DriectUIlib.h"

using namespace DuiLib;

#pragma warning(disable:4190)



typedef LRESULT(*HandleMessageCallBack)(LPVOID, UINT, WPARAM, LPARAM, BOOL&);
typedef void(*NotifyCallBack)(LPVOID, TNotifyUI&);
typedef void(*FinalMessageCallBack)(LPVOID, HWND);
typedef LRESULT(*MessageCallBack)(LPVOID, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
typedef void(*InitWindowCallBack)(LPVOID);
typedef CControlUI*(*CreateControlCallBack)(LPVOID, LPCTSTR);
typedef LPCTSTR(*GetItemTextCallBack)(LPVOID, CControlUI*, int, int);

class CDelphi_WindowImplBase : // 好吧，你赢了,我认输
	                           public IListCallbackUI, //这个貌似只能放第一个，他取的时候就是取第一个的，要不就另行建立一个类
							   public WindowImplBase
{
protected:
	LPVOID m_Self;
	LPCTSTR m_ClassName;
	LPCTSTR m_SkinFile;
	LPCTSTR m_SkinFolder;
	LPCTSTR m_ZipFileName;
	UINT m_GetClassStyle;
	UILIB_RESOURCETYPE m_RType;
	InitWindowCallBack m_InitWindow;
	FinalMessageCallBack m_FinalMessage;
	HandleMessageCallBack m_HandleMessage;
	NotifyCallBack m_Notify;
	NotifyCallBack m_Click;
	MessageCallBack m_MessageHandler;
	MessageCallBack m_HandleCustomMessage;
	CreateControlCallBack m_CreateControl;
	GetItemTextCallBack m_GetItemText;
public:
	CDelphi_WindowImplBase() :
		WindowImplBase(),
		m_Self(NULL),
		m_ClassName(NULL),
		m_SkinFile(NULL),
		m_SkinFolder(NULL),
		m_ZipFileName(NULL),
		m_RType(UILIB_FILE),
		m_InitWindow(NULL),
		m_FinalMessage(NULL),
		m_HandleMessage(NULL),
		m_Notify(NULL),
		m_Click(NULL),
		m_MessageHandler(NULL),
		m_HandleCustomMessage(NULL),
		m_GetItemText(NULL) {
		m_GetClassStyle = WindowImplBase::GetClassStyle();
	}
	~CDelphi_WindowImplBase(){ };
	void InitWindow()
	{
		if (m_InitWindow)
			m_InitWindow(m_Self);
	}
	void OnFinalMessage(HWND hWnd)
	{
		//WindowImplBase::OnFinalMessage(hWnd); // 另作处理，不然有些窗口不想关的结果资源被释放了
		if (m_FinalMessage)
			m_FinalMessage(m_Self, hWnd);
	}
	void Notify(TNotifyUI& msg)
	{
		if (m_Notify)
			m_Notify(m_Self, msg);
		return WindowImplBase::Notify(msg);
	}
	void OnClick(TNotifyUI& msg)
	{
		if (m_Click)
			m_Click(m_Self, msg);
	}
	CControlUI* CreateControl(LPCTSTR pstrClass) {
		if (m_CreateControl)
			return	m_CreateControl(m_Self, pstrClass);
		return NULL;
	}
    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem) {
		if (m_GetItemText)
			return m_GetItemText(m_Self, pControl, iIndex, iSubItem);
		return NULL;
	}
public:
	LPCTSTR GetWindowClassName() const { return m_ClassName; }
	CDuiString GetSkinFile() { return m_SkinFile; };
	CDuiString GetSkinFolder() { return m_SkinFolder; };
	CDuiString GetZIPFileName() const { return m_ZipFileName; };
	UILIB_RESOURCETYPE GetResourceType() const { return m_RType; };
	UINT GetClassStyle() const { return m_GetClassStyle; };
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = 0;
		if (m_MessageHandler)
		{
			lRes = m_MessageHandler(m_Self, uMsg, wParam, lParam, bHandled);
			if (lRes) return lRes;
		}
		bool b = (bool)bHandled;
		lRes = WindowImplBase::MessageHandler(uMsg, wParam, lParam, b);
		bHandled = (BOOL)b;
		return lRes;
	}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_HandleMessage)
		{
			// 这样是为了规避原来的那个事件问题
			BOOL bHandled = TRUE;
			LRESULT lRes = m_HandleMessage(m_Self, uMsg, wParam, lParam, bHandled);
			if (!bHandled) {
	
				lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
				if (bHandled) return lRes;

				if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
					return lRes;
				// 绕过父类的HandleMessage直接调用父类的父类方法
				return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			}
			if (lRes) return lRes;
		}
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	}
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_HandleCustomMessage)
		{
			LRESULT lRes = m_HandleCustomMessage(m_Self, uMsg, wParam, lParam, bHandled);
			if (lRes) return lRes;
		}
		return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	};
public:
	CPaintManagerUI* GetPaintManagerUI() { return &m_PaintManager; };
	void SetDelphiSelf(LPVOID Self) { m_Self = Self; }
	void SetClassName(LPCTSTR ClassName) { m_ClassName = ClassName; }
	void SetSkinFile(LPCTSTR SkinFile) { m_SkinFile = SkinFile; }
	void SetSkinFolder(LPCTSTR SkinFolder) { m_SkinFolder = SkinFolder; }
	void SetZipFileName(LPCTSTR ZipFileName) { m_ZipFileName = ZipFileName; }
	void SetResourceType(UILIB_RESOURCETYPE RType) { m_RType = RType; }
	void SetInitWindow(InitWindowCallBack Callback) { m_InitWindow = Callback; }
	void SetFinalMessage(FinalMessageCallBack Callback) { m_FinalMessage = Callback; }
	void SetHandleMessage(HandleMessageCallBack Callback) { m_HandleMessage = Callback; }
	void SetNotify(NotifyCallBack Callback) { m_Notify = Callback; }
	void SetClick(NotifyCallBack Callback) { m_Click = Callback; }
	void SetMessageHandler(MessageCallBack Callback) { m_MessageHandler = Callback; }
	void SetHandleCustomMessage(MessageCallBack Callback) { m_HandleCustomMessage = Callback; }
	void SetCreateControl(CreateControlCallBack CallBack) { m_CreateControl = CallBack; }
	void SetGetItemText(GetItemTextCallBack ACallBack) {
		m_GetItemText = ACallBack;
	}
	void SetGetClassStyle(UINT uStyle) { m_GetClassStyle = uStyle; };
	void RemoveThisInPaintManager() {
		m_PaintManager.RemovePreMessageFilter(this);
		m_PaintManager.RemoveNotifier(this);
		m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
	}
};


typedef void(*DoEventCallBack)(LPVOID, TEventUI&);
class CDelphi_ListUI : public CListUI {
protected:
	LPVOID m_Self;
	DoEventCallBack m_DoEvent;
public:
	CDelphi_ListUI() :
		m_Self(NULL),
		m_DoEvent(NULL)
	{
	}
	void DoEvent(TEventUI& event) {
		if (m_DoEvent)
			m_DoEvent(m_Self, event);
		CListUI::DoEvent(event);
	}
public:
	void SetDelphiSelf(LPVOID ASelf) { m_Self = ASelf; }
	void SetDoEvent(DoEventCallBack ADoEvent) { m_DoEvent = ADoEvent; }
};



//================================CStdStringPtrMap============================

DRIECTUILIB_API CStdStringPtrMap* Delphi_StdStringPtrMap_CppCreate() {
    return new CStdStringPtrMap();
}

DRIECTUILIB_API void Delphi_StdStringPtrMap_CppDestroy(CStdStringPtrMap* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_StdStringPtrMap_Resize(CStdStringPtrMap* handle ,int nSize) {
    handle->Resize(nSize);
}

DRIECTUILIB_API LPVOID Delphi_StdStringPtrMap_Find(CStdStringPtrMap* handle ,LPCTSTR key, bool optimize) {
    return handle->Find(key, optimize);
}

DRIECTUILIB_API bool Delphi_StdStringPtrMap_Insert(CStdStringPtrMap* handle ,LPCTSTR key, LPVOID pData) {
    return handle->Insert(key, pData);
}

DRIECTUILIB_API LPVOID Delphi_StdStringPtrMap_Set(CStdStringPtrMap* handle ,LPCTSTR key, LPVOID pData) {
    return handle->Set(key, pData);
}

DRIECTUILIB_API bool Delphi_StdStringPtrMap_Remove(CStdStringPtrMap* handle ,LPCTSTR key) {
    return handle->Remove(key);
}

DRIECTUILIB_API void Delphi_StdStringPtrMap_RemoveAll(CStdStringPtrMap* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API int Delphi_StdStringPtrMap_GetSize(CStdStringPtrMap* handle) {
    return handle->GetSize();
}

DRIECTUILIB_API LPCTSTR Delphi_StdStringPtrMap_GetAt(CStdStringPtrMap* handle ,int iIndex) {
    return handle->GetAt(iIndex);
}

//================================CStdValArray============================

DRIECTUILIB_API CStdValArray* Delphi_StdValArray_CppCreate(int iElementSize, int iPreallocSize) {
    return new CStdValArray(iElementSize, iPreallocSize);
}

DRIECTUILIB_API void Delphi_StdValArray_CppDestroy(CStdValArray* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_StdValArray_Empty(CStdValArray* handle) {
    handle->Empty();
}

DRIECTUILIB_API bool Delphi_StdValArray_IsEmpty(CStdValArray* handle) {
    return handle->IsEmpty();
}

DRIECTUILIB_API bool Delphi_StdValArray_Add(CStdValArray* handle ,LPCVOID pData) {
    return handle->Add(pData);
}

DRIECTUILIB_API bool Delphi_StdValArray_Remove(CStdValArray* handle ,int iIndex) {
    return handle->Remove(iIndex);
}

DRIECTUILIB_API int Delphi_StdValArray_GetSize(CStdValArray* handle) {
    return handle->GetSize();
}

DRIECTUILIB_API LPVOID Delphi_StdValArray_GetData(CStdValArray* handle) {
    return handle->GetData();
}

DRIECTUILIB_API LPVOID Delphi_StdValArray_GetAt(CStdValArray* handle ,int iIndex) {
    return handle->GetAt(iIndex);
}

//================================CStdPtrArray============================

DRIECTUILIB_API CStdPtrArray* Delphi_StdPtrArray_CppCreate() {
    return new CStdPtrArray();
}

DRIECTUILIB_API void Delphi_StdPtrArray_CppDestroy(CStdPtrArray* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_StdPtrArray_Empty(CStdPtrArray* handle) {
    handle->Empty();
}

DRIECTUILIB_API void Delphi_StdPtrArray_Resize(CStdPtrArray* handle ,int iSize) {
    handle->Resize(iSize);
}

DRIECTUILIB_API bool Delphi_StdPtrArray_IsEmpty(CStdPtrArray* handle) {
    return handle->IsEmpty();
}

DRIECTUILIB_API int Delphi_StdPtrArray_Find(CStdPtrArray* handle ,LPVOID iIndex) {
    return handle->Find(iIndex);
}

DRIECTUILIB_API bool Delphi_StdPtrArray_Add(CStdPtrArray* handle ,LPVOID pData) {
    return handle->Add(pData);
}

DRIECTUILIB_API bool Delphi_StdPtrArray_SetAt(CStdPtrArray* handle ,int iIndex, LPVOID pData) {
    return handle->SetAt(iIndex, pData);
}

DRIECTUILIB_API bool Delphi_StdPtrArray_InsertAt(CStdPtrArray* handle ,int iIndex, LPVOID pData) {
    return handle->InsertAt(iIndex, pData);
}

DRIECTUILIB_API bool Delphi_StdPtrArray_Remove(CStdPtrArray* handle ,int iIndex) {
    return handle->Remove(iIndex);
}

DRIECTUILIB_API int Delphi_StdPtrArray_GetSize(CStdPtrArray* handle) {
    return handle->GetSize();
}

DRIECTUILIB_API LPVOID* Delphi_StdPtrArray_GetData(CStdPtrArray* handle) {
    return handle->GetData();
}

DRIECTUILIB_API LPVOID Delphi_StdPtrArray_GetAt(CStdPtrArray* handle ,int iIndex) {
    return handle->GetAt(iIndex);
}

//================================CNotifyPump============================

DRIECTUILIB_API CNotifyPump* Delphi_NotifyPump_CppCreate() {
    return new CNotifyPump();
}

DRIECTUILIB_API void Delphi_NotifyPump_CppDestroy(CNotifyPump* handle) {
    delete handle;
}

DRIECTUILIB_API bool Delphi_NotifyPump_AddVirtualWnd(CNotifyPump* handle ,CDuiString strName, CNotifyPump* pObject) {
    return handle->AddVirtualWnd(strName, pObject);
}

DRIECTUILIB_API bool Delphi_NotifyPump_RemoveVirtualWnd(CNotifyPump* handle ,CDuiString strName) {
    return handle->RemoveVirtualWnd(strName);
}

DRIECTUILIB_API void Delphi_NotifyPump_NotifyPump(CNotifyPump* handle ,TNotifyUI& msg) {
    handle->NotifyPump(msg);
}

DRIECTUILIB_API bool Delphi_NotifyPump_LoopDispatch(CNotifyPump* handle ,TNotifyUI& msg) {
    return handle->LoopDispatch(msg);
}

//================================CDialogBuilder============================

DRIECTUILIB_API CDialogBuilder* Delphi_DialogBuilder_CppCreate() {
    return new CDialogBuilder();
}

DRIECTUILIB_API void Delphi_DialogBuilder_CppDestroy(CDialogBuilder* handle) {
    delete handle;
}

DRIECTUILIB_API CControlUI* Delphi_DialogBuilder_Create_01(CDialogBuilder* handle ,STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent) {
    return handle->Create(xml, type, pCallback, pManager, pParent);
}

DRIECTUILIB_API CControlUI* Delphi_DialogBuilder_Create_02(CDialogBuilder* handle ,IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent) {
    return handle->Create(pCallback, pManager, pParent);
}

DRIECTUILIB_API CMarkup* Delphi_DialogBuilder_GetMarkup(CDialogBuilder* handle) {
    return handle->GetMarkup();
}

DRIECTUILIB_API void Delphi_DialogBuilder_GetLastErrorMessage(CDialogBuilder* handle ,LPTSTR pstrMessage, SIZE_T cchMax) {
    handle->GetLastErrorMessage(pstrMessage, cchMax);
}

DRIECTUILIB_API void Delphi_DialogBuilder_GetLastErrorLocation(CDialogBuilder* handle ,LPTSTR pstrSource, SIZE_T cchMax) {
    handle->GetLastErrorLocation(pstrSource, cchMax);
}

//================================CMarkup============================

DRIECTUILIB_API CMarkup* Delphi_Markup_CppCreate(LPCTSTR pstrXML) {
    return new CMarkup(pstrXML);
}

DRIECTUILIB_API void Delphi_Markup_CppDestroy(CMarkup* handle) {
    delete handle;
}

DRIECTUILIB_API bool Delphi_Markup_Load(CMarkup* handle ,LPCTSTR pstrXML) {
    return handle->Load(pstrXML);
}

DRIECTUILIB_API bool Delphi_Markup_LoadFromMem(CMarkup* handle ,BYTE* pByte, DWORD dwSize, int encoding) {
    return handle->LoadFromMem(pByte, dwSize, encoding);
}

DRIECTUILIB_API bool Delphi_Markup_LoadFromFile(CMarkup* handle ,LPCTSTR pstrFilename, int encoding) {
    return handle->LoadFromFile(pstrFilename, encoding);
}

DRIECTUILIB_API void Delphi_Markup_Release(CMarkup* handle) {
    handle->Release();
}

DRIECTUILIB_API bool Delphi_Markup_IsValid(CMarkup* handle) {
    return handle->IsValid();
}

DRIECTUILIB_API void Delphi_Markup_SetPreserveWhitespace(CMarkup* handle ,bool bPreserve) {
    handle->SetPreserveWhitespace(bPreserve);
}

DRIECTUILIB_API void Delphi_Markup_GetLastErrorMessage(CMarkup* handle ,LPTSTR pstrMessage, SIZE_T cchMax) {
    handle->GetLastErrorMessage(pstrMessage, cchMax);
}

DRIECTUILIB_API void Delphi_Markup_GetLastErrorLocation(CMarkup* handle ,LPTSTR pstrSource, SIZE_T cchMax) {
    handle->GetLastErrorLocation(pstrSource, cchMax);
}

DRIECTUILIB_API CMarkupNode Delphi_Markup_GetRoot(CMarkup* handle) {
    return handle->GetRoot();
}

//================================CMarkupNode============================

DRIECTUILIB_API CMarkupNode* Delphi_MarkupNode_CppCreate() {
    return new CMarkupNode();
}

DRIECTUILIB_API void Delphi_MarkupNode_CppDestroy(CMarkupNode* handle) {
    delete handle;
}

DRIECTUILIB_API bool Delphi_MarkupNode_IsValid(CMarkupNode* handle) {
    return handle->IsValid();
}

DRIECTUILIB_API CMarkupNode Delphi_MarkupNode_GetParent(CMarkupNode* handle) {
    return handle->GetParent();
}

DRIECTUILIB_API CMarkupNode Delphi_MarkupNode_GetSibling(CMarkupNode* handle) {
    return handle->GetSibling();
}

DRIECTUILIB_API CMarkupNode Delphi_MarkupNode_GetChild_01(CMarkupNode* handle) {
    return handle->GetChild();
}

DRIECTUILIB_API CMarkupNode Delphi_MarkupNode_GetChild_02(CMarkupNode* handle ,LPCTSTR pstrName) {
    return handle->GetChild(pstrName);
}

DRIECTUILIB_API bool Delphi_MarkupNode_HasSiblings(CMarkupNode* handle) {
    return handle->HasSiblings();
}

DRIECTUILIB_API bool Delphi_MarkupNode_HasChildren(CMarkupNode* handle) {
    return handle->HasChildren();
}

DRIECTUILIB_API LPCTSTR Delphi_MarkupNode_GetName(CMarkupNode* handle) {
    return handle->GetName();
}

DRIECTUILIB_API LPCTSTR Delphi_MarkupNode_GetValue(CMarkupNode* handle) {
    return handle->GetValue();
}

DRIECTUILIB_API bool Delphi_MarkupNode_HasAttributes(CMarkupNode* handle) {
    return handle->HasAttributes();
}

DRIECTUILIB_API bool Delphi_MarkupNode_HasAttribute(CMarkupNode* handle ,LPCTSTR pstrName) {
    return handle->HasAttribute(pstrName);
}

DRIECTUILIB_API int Delphi_MarkupNode_GetAttributeCount(CMarkupNode* handle) {
    return handle->GetAttributeCount();
}

DRIECTUILIB_API LPCTSTR Delphi_MarkupNode_GetAttributeName(CMarkupNode* handle ,int iIndex) {
    return handle->GetAttributeName(iIndex);
}

DRIECTUILIB_API LPCTSTR Delphi_MarkupNode_GetAttributeValue_01(CMarkupNode* handle ,int iIndex) {
    return handle->GetAttributeValue(iIndex);
}

DRIECTUILIB_API LPCTSTR Delphi_MarkupNode_GetAttributeValue_02(CMarkupNode* handle ,LPCTSTR pstrName) {
    return handle->GetAttributeValue(pstrName);
}

DRIECTUILIB_API bool Delphi_MarkupNode_GetAttributeValue_03(CMarkupNode* handle ,int iIndex, LPTSTR pstrValue, SIZE_T cchMax) {
    return handle->GetAttributeValue(iIndex, pstrValue, cchMax);
}

DRIECTUILIB_API bool Delphi_MarkupNode_GetAttributeValue_04(CMarkupNode* handle ,LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax) {
    return handle->GetAttributeValue(pstrName, pstrValue, cchMax);
}

//================================CControlUI============================

DRIECTUILIB_API CControlUI* Delphi_ControlUI_CppCreate() {
    return new CControlUI();
}

DRIECTUILIB_API void Delphi_ControlUI_CppDestroy(CControlUI* handle) {
    delete handle;
}

DRIECTUILIB_API CDuiString Delphi_ControlUI_GetName(CControlUI* handle) {
    return handle->GetName();
}

DRIECTUILIB_API void Delphi_ControlUI_SetName(CControlUI* handle ,LPCTSTR pstrName) {
    handle->SetName(pstrName);
}

DRIECTUILIB_API LPCTSTR Delphi_ControlUI_GetClass(CControlUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ControlUI_GetInterface(CControlUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_ControlUI_GetControlFlags(CControlUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API bool Delphi_ControlUI_Activate(CControlUI* handle) {
    return handle->Activate();
}

DRIECTUILIB_API CPaintManagerUI* Delphi_ControlUI_GetManager(CControlUI* handle) {
    return handle->GetManager();
}

DRIECTUILIB_API void Delphi_ControlUI_SetManager(CControlUI* handle ,CPaintManagerUI* pManager, CControlUI* pParent, bool bInit) {
    handle->SetManager(pManager, pParent, bInit);
}

DRIECTUILIB_API CControlUI* Delphi_ControlUI_GetParent(CControlUI* handle) {
    return handle->GetParent();
}

DRIECTUILIB_API CDuiString Delphi_ControlUI_GetText(CControlUI* handle) {
    return handle->GetText();
}

DRIECTUILIB_API void Delphi_ControlUI_SetText(CControlUI* handle ,LPCTSTR pstrText) {
    handle->SetText(pstrText);
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetBkColor(CControlUI* handle) {
    return handle->GetBkColor();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBkColor(CControlUI* handle ,DWORD dwBackColor) {
    handle->SetBkColor(dwBackColor);
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetBkColor2(CControlUI* handle) {
    return handle->GetBkColor2();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBkColor2(CControlUI* handle ,DWORD dwBackColor) {
    handle->SetBkColor2(dwBackColor);
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetBkColor3(CControlUI* handle) {
    return handle->GetBkColor3();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBkColor3(CControlUI* handle ,DWORD dwBackColor) {
    handle->SetBkColor3(dwBackColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ControlUI_GetBkImage(CControlUI* handle) {
    return handle->GetBkImage();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBkImage(CControlUI* handle ,LPCTSTR pStrImage) {
    handle->SetBkImage(pStrImage);
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetFocusBorderColor(CControlUI* handle) {
    return handle->GetFocusBorderColor();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFocusBorderColor(CControlUI* handle ,DWORD dwBorderColor) {
    handle->SetFocusBorderColor(dwBorderColor);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsColorHSL(CControlUI* handle) {
    return handle->IsColorHSL();
}

DRIECTUILIB_API void Delphi_ControlUI_SetColorHSL(CControlUI* handle ,bool bColorHSL) {
    handle->SetColorHSL(bColorHSL);
}

DRIECTUILIB_API void Delphi_ControlUI_GetBorderRound(CControlUI* handle, SIZE& Result) {
    Result = handle->GetBorderRound();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBorderRound(CControlUI* handle ,SIZE cxyRound) {
    handle->SetBorderRound(cxyRound);
}

DRIECTUILIB_API bool Delphi_ControlUI_DrawImage(CControlUI* handle ,HDC hDC, TDrawInfo& drawInfo) {
    return handle->DrawImage(hDC, drawInfo);
}

DRIECTUILIB_API int Delphi_ControlUI_GetBorderSize(CControlUI* handle) {
    return handle->GetBorderSize();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBorderSize_01(CControlUI* handle ,int nSize) {
    handle->SetBorderSize(nSize);
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetBorderColor(CControlUI* handle) {
    return handle->GetBorderColor();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBorderColor(CControlUI* handle ,DWORD dwBorderColor) {
    handle->SetBorderColor(dwBorderColor);
}

DRIECTUILIB_API void Delphi_ControlUI_SetBorderSize_02(CControlUI* handle ,RECT rc) {
    handle->SetBorderSize(rc);
}

DRIECTUILIB_API int Delphi_ControlUI_GetLeftBorderSize(CControlUI* handle) {
    return handle->GetLeftBorderSize();
}

DRIECTUILIB_API void Delphi_ControlUI_SetLeftBorderSize(CControlUI* handle ,int nSize) {
    handle->SetLeftBorderSize(nSize);
}

DRIECTUILIB_API int Delphi_ControlUI_GetTopBorderSize(CControlUI* handle) {
    return handle->GetTopBorderSize();
}

DRIECTUILIB_API void Delphi_ControlUI_SetTopBorderSize(CControlUI* handle ,int nSize) {
    handle->SetTopBorderSize(nSize);
}

DRIECTUILIB_API int Delphi_ControlUI_GetRightBorderSize(CControlUI* handle) {
    return handle->GetRightBorderSize();
}

DRIECTUILIB_API void Delphi_ControlUI_SetRightBorderSize(CControlUI* handle ,int nSize) {
    handle->SetRightBorderSize(nSize);
}

DRIECTUILIB_API int Delphi_ControlUI_GetBottomBorderSize(CControlUI* handle) {
    return handle->GetBottomBorderSize();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBottomBorderSize(CControlUI* handle ,int nSize) {
    handle->SetBottomBorderSize(nSize);
}

DRIECTUILIB_API int Delphi_ControlUI_GetBorderStyle(CControlUI* handle) {
    return handle->GetBorderStyle();
}

DRIECTUILIB_API void Delphi_ControlUI_SetBorderStyle(CControlUI* handle ,int nStyle) {
    handle->SetBorderStyle(nStyle);
}

DRIECTUILIB_API const RECT& Delphi_ControlUI_GetPos(CControlUI* handle) {
    return handle->GetPos();
}

DRIECTUILIB_API void Delphi_ControlUI_GetRelativePos(CControlUI* handle, RECT& Result) {
    Result = handle->GetRelativePos();
}

DRIECTUILIB_API void Delphi_ControlUI_SetPos(CControlUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ControlUI_Move(CControlUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API int Delphi_ControlUI_GetWidth(CControlUI* handle) {
    return handle->GetWidth();
}

DRIECTUILIB_API int Delphi_ControlUI_GetHeight(CControlUI* handle) {
    return handle->GetHeight();
}

DRIECTUILIB_API int Delphi_ControlUI_GetX(CControlUI* handle) {
    return handle->GetX();
}

DRIECTUILIB_API int Delphi_ControlUI_GetY(CControlUI* handle) {
    return handle->GetY();
}

DRIECTUILIB_API void Delphi_ControlUI_GetPadding(CControlUI* handle, RECT& Result) {
    Result = handle->GetPadding();
}

DRIECTUILIB_API void Delphi_ControlUI_SetPadding(CControlUI* handle ,RECT rcPadding) {
    handle->SetPadding(rcPadding);
}

DRIECTUILIB_API void Delphi_ControlUI_GetFixedXY(CControlUI* handle, SIZE& Result) {
    Result = handle->GetFixedXY();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFixedXY(CControlUI* handle ,SIZE szXY) {
    handle->SetFixedXY(szXY);
}

DRIECTUILIB_API int Delphi_ControlUI_GetFixedWidth(CControlUI* handle) {
    return handle->GetFixedWidth();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFixedWidth(CControlUI* handle ,int cx) {
    handle->SetFixedWidth(cx);
}

DRIECTUILIB_API int Delphi_ControlUI_GetFixedHeight(CControlUI* handle) {
    return handle->GetFixedHeight();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFixedHeight(CControlUI* handle ,int cy) {
    handle->SetFixedHeight(cy);
}

DRIECTUILIB_API int Delphi_ControlUI_GetMinWidth(CControlUI* handle) {
    return handle->GetMinWidth();
}

DRIECTUILIB_API void Delphi_ControlUI_SetMinWidth(CControlUI* handle ,int cx) {
    handle->SetMinWidth(cx);
}

DRIECTUILIB_API int Delphi_ControlUI_GetMaxWidth(CControlUI* handle) {
    return handle->GetMaxWidth();
}

DRIECTUILIB_API void Delphi_ControlUI_SetMaxWidth(CControlUI* handle ,int cx) {
    handle->SetMaxWidth(cx);
}

DRIECTUILIB_API int Delphi_ControlUI_GetMinHeight(CControlUI* handle) {
    return handle->GetMinHeight();
}

DRIECTUILIB_API void Delphi_ControlUI_SetMinHeight(CControlUI* handle ,int cy) {
    handle->SetMinHeight(cy);
}

DRIECTUILIB_API int Delphi_ControlUI_GetMaxHeight(CControlUI* handle) {
    return handle->GetMaxHeight();
}

DRIECTUILIB_API void Delphi_ControlUI_SetMaxHeight(CControlUI* handle ,int cy) {
    handle->SetMaxHeight(cy);
}

DRIECTUILIB_API TPercentInfo Delphi_ControlUI_GetFloatPercent(CControlUI* handle) {
    return handle->GetFloatPercent();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFloatPercent(CControlUI* handle ,TPercentInfo piFloatPercent) {
    handle->SetFloatPercent(piFloatPercent);
}

DRIECTUILIB_API CDuiString Delphi_ControlUI_GetToolTip(CControlUI* handle) {
    return handle->GetToolTip();
}

DRIECTUILIB_API void Delphi_ControlUI_SetToolTip(CControlUI* handle ,LPCTSTR pstrText) {
    handle->SetToolTip(pstrText);
}

DRIECTUILIB_API void Delphi_ControlUI_SetToolTipWidth(CControlUI* handle ,int nWidth) {
    handle->SetToolTipWidth(nWidth);
}

DRIECTUILIB_API int Delphi_ControlUI_GetToolTipWidth(CControlUI* handle) {
    return handle->GetToolTipWidth();
}

DRIECTUILIB_API TCHAR Delphi_ControlUI_GetShortcut(CControlUI* handle) {
    return handle->GetShortcut();
}

DRIECTUILIB_API void Delphi_ControlUI_SetShortcut(CControlUI* handle ,TCHAR ch) {
    handle->SetShortcut(ch);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsContextMenuUsed(CControlUI* handle) {
    return handle->IsContextMenuUsed();
}

DRIECTUILIB_API void Delphi_ControlUI_SetContextMenuUsed(CControlUI* handle ,bool bMenuUsed) {
    handle->SetContextMenuUsed(bMenuUsed);
}

DRIECTUILIB_API const CDuiString& Delphi_ControlUI_GetUserData(CControlUI* handle) {
    return handle->GetUserData();
}

DRIECTUILIB_API void Delphi_ControlUI_SetUserData(CControlUI* handle ,LPCTSTR pstrText) {
    handle->SetUserData(pstrText);
}

DRIECTUILIB_API UINT_PTR Delphi_ControlUI_GetTag(CControlUI* handle) {
    return handle->GetTag();
}

DRIECTUILIB_API void Delphi_ControlUI_SetTag(CControlUI* handle ,UINT_PTR pTag) {
    handle->SetTag(pTag);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsVisible(CControlUI* handle) {
    return handle->IsVisible();
}

DRIECTUILIB_API void Delphi_ControlUI_SetVisible(CControlUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ControlUI_SetInternVisible(CControlUI* handle ,bool bVisible) {
    handle->SetInternVisible(bVisible);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsEnabled(CControlUI* handle) {
    return handle->IsEnabled();
}

DRIECTUILIB_API void Delphi_ControlUI_SetEnabled(CControlUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsMouseEnabled(CControlUI* handle) {
    return handle->IsMouseEnabled();
}

DRIECTUILIB_API void Delphi_ControlUI_SetMouseEnabled(CControlUI* handle ,bool bEnable) {
    handle->SetMouseEnabled(bEnable);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsKeyboardEnabled(CControlUI* handle) {
    return handle->IsKeyboardEnabled();
}

DRIECTUILIB_API void Delphi_ControlUI_SetKeyboardEnabled(CControlUI* handle ,bool bEnable) {
    handle->SetKeyboardEnabled(bEnable);
}

DRIECTUILIB_API bool Delphi_ControlUI_IsFocused(CControlUI* handle) {
    return handle->IsFocused();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFocus(CControlUI* handle) {
    handle->SetFocus();
}

DRIECTUILIB_API bool Delphi_ControlUI_IsFloat(CControlUI* handle) {
    return handle->IsFloat();
}

DRIECTUILIB_API void Delphi_ControlUI_SetFloat(CControlUI* handle ,bool bFloat) {
    handle->SetFloat(bFloat);
}

DRIECTUILIB_API CControlUI* Delphi_ControlUI_FindControl(CControlUI* handle ,FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags) {
    return handle->FindControl(Proc, pData, uFlags);
}

DRIECTUILIB_API void Delphi_ControlUI_Invalidate(CControlUI* handle) {
    handle->Invalidate();
}

DRIECTUILIB_API bool Delphi_ControlUI_IsUpdateNeeded(CControlUI* handle) {
    return handle->IsUpdateNeeded();
}

DRIECTUILIB_API void Delphi_ControlUI_NeedUpdate(CControlUI* handle) {
    handle->NeedUpdate();
}

DRIECTUILIB_API void Delphi_ControlUI_NeedParentUpdate(CControlUI* handle) {
    handle->NeedParentUpdate();
}

DRIECTUILIB_API DWORD Delphi_ControlUI_GetAdjustColor(CControlUI* handle ,DWORD dwColor) {
    return handle->GetAdjustColor(dwColor);
}

DRIECTUILIB_API void Delphi_ControlUI_Init(CControlUI* handle) {
    handle->Init();
}

DRIECTUILIB_API void Delphi_ControlUI_DoInit(CControlUI* handle) {
    handle->DoInit();
}

DRIECTUILIB_API void Delphi_ControlUI_Event(CControlUI* handle ,TEventUI& event) {
    handle->Event(event);
}

DRIECTUILIB_API void Delphi_ControlUI_DoEvent(CControlUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ControlUI_SetAttribute(CControlUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API CControlUI* Delphi_ControlUI_ApplyAttributeList(CControlUI* handle ,LPCTSTR pstrList) {
    return handle->ApplyAttributeList(pstrList);
}

DRIECTUILIB_API void Delphi_ControlUI_EstimateSize(CControlUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_ControlUI_DoPaint(CControlUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ControlUI_PaintBkColor(CControlUI* handle ,HDC hDC) {
    handle->PaintBkColor(hDC);
}

DRIECTUILIB_API void Delphi_ControlUI_PaintBkImage(CControlUI* handle ,HDC hDC) {
    handle->PaintBkImage(hDC);
}

DRIECTUILIB_API void Delphi_ControlUI_PaintStatusImage(CControlUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

DRIECTUILIB_API void Delphi_ControlUI_PaintText(CControlUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

DRIECTUILIB_API void Delphi_ControlUI_PaintBorder(CControlUI* handle ,HDC hDC) {
    handle->PaintBorder(hDC);
}

DRIECTUILIB_API void Delphi_ControlUI_DoPostPaint(CControlUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPostPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ControlUI_SetVirtualWnd(CControlUI* handle ,LPCTSTR pstrValue) {
    handle->SetVirtualWnd(pstrValue);
}

DRIECTUILIB_API CDuiString Delphi_ControlUI_GetVirtualWnd(CControlUI* handle) {
    return handle->GetVirtualWnd();
}

//================================CDelphi_WindowImplBase============================

DRIECTUILIB_API CDelphi_WindowImplBase* Delphi_Delphi_WindowImplBase_CppCreate() {
    return new CDelphi_WindowImplBase();
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_CppDestroy(CDelphi_WindowImplBase* handle) {
    delete handle;
}

DRIECTUILIB_API HWND Delphi_Delphi_WindowImplBase_GetHWND(CDelphi_WindowImplBase* handle) {
    return handle->GetHWND();
}

DRIECTUILIB_API bool Delphi_Delphi_WindowImplBase_RegisterWindowClass(CDelphi_WindowImplBase* handle) {
    return handle->RegisterWindowClass();
}

DRIECTUILIB_API bool Delphi_Delphi_WindowImplBase_RegisterSuperclass(CDelphi_WindowImplBase* handle) {
    return handle->RegisterSuperclass();
}

DRIECTUILIB_API HWND Delphi_Delphi_WindowImplBase_Create_01(CDelphi_WindowImplBase* handle ,HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, RECT rc, HMENU hMenu) {
    return handle->Create(hwndParent, pstrName, dwStyle, dwExStyle, rc, hMenu);
}

DRIECTUILIB_API HWND Delphi_Delphi_WindowImplBase_Create_02(CDelphi_WindowImplBase* handle ,HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu) {
    return handle->Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
}

DRIECTUILIB_API HWND Delphi_Delphi_WindowImplBase_CreateDuiWindow(CDelphi_WindowImplBase* handle ,HWND hwndParent, LPCTSTR pstrWindowName, DWORD dwStyle, DWORD dwExStyle) {
    return handle->CreateDuiWindow(hwndParent, pstrWindowName, dwStyle, dwExStyle);
}

DRIECTUILIB_API HWND Delphi_Delphi_WindowImplBase_Subclass(CDelphi_WindowImplBase* handle ,HWND hWnd) {
    return handle->Subclass(hWnd);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_Unsubclass(CDelphi_WindowImplBase* handle) {
    handle->Unsubclass();
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_ShowWindow(CDelphi_WindowImplBase* handle ,bool bShow, bool bTakeFocus) {
    handle->ShowWindow(bShow, bTakeFocus);
}

DRIECTUILIB_API UINT Delphi_Delphi_WindowImplBase_ShowModal(CDelphi_WindowImplBase* handle) {
    return handle->ShowModal();
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_Close(CDelphi_WindowImplBase* handle ,UINT nRet) {
    handle->Close(nRet);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_CenterWindow(CDelphi_WindowImplBase* handle) {
    handle->CenterWindow();
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetIcon(CDelphi_WindowImplBase* handle ,UINT nRes) {
    handle->SetIcon(nRes);
}

DRIECTUILIB_API LRESULT Delphi_Delphi_WindowImplBase_SendMessage(CDelphi_WindowImplBase* handle ,UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return handle->SendMessage(uMsg, wParam, lParam);
}

DRIECTUILIB_API LRESULT Delphi_Delphi_WindowImplBase_PostMessage(CDelphi_WindowImplBase* handle ,UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return handle->PostMessage(uMsg, wParam, lParam);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_ResizeClient(CDelphi_WindowImplBase* handle ,int cx, int cy) {
    handle->ResizeClient(cx, cy);
}

DRIECTUILIB_API bool Delphi_Delphi_WindowImplBase_AddVirtualWnd(CDelphi_WindowImplBase* handle ,CDuiString strName, CNotifyPump* pObject) {
    return handle->AddVirtualWnd(strName, pObject);
}

DRIECTUILIB_API bool Delphi_Delphi_WindowImplBase_RemoveVirtualWnd(CDelphi_WindowImplBase* handle ,CDuiString strName) {
    return handle->RemoveVirtualWnd(strName);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_NotifyPump(CDelphi_WindowImplBase* handle ,TNotifyUI& msg) {
    handle->NotifyPump(msg);
}

DRIECTUILIB_API bool Delphi_Delphi_WindowImplBase_LoopDispatch(CDelphi_WindowImplBase* handle ,TNotifyUI& msg) {
    return handle->LoopDispatch(msg);
}

DRIECTUILIB_API CPaintManagerUI* Delphi_Delphi_WindowImplBase_GetPaintManagerUI(CDelphi_WindowImplBase* handle) {
    return handle->GetPaintManagerUI();
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetDelphiSelf(CDelphi_WindowImplBase* handle ,LPVOID Self) {
    handle->SetDelphiSelf(Self);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetClassName(CDelphi_WindowImplBase* handle ,LPCTSTR ClassName) {
    handle->SetClassName(ClassName);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetSkinFile(CDelphi_WindowImplBase* handle ,LPCTSTR SkinFile) {
    handle->SetSkinFile(SkinFile);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetSkinFolder(CDelphi_WindowImplBase* handle ,LPCTSTR SkinFolder) {
    handle->SetSkinFolder(SkinFolder);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetZipFileName(CDelphi_WindowImplBase* handle ,LPCTSTR ZipFileName) {
    handle->SetZipFileName(ZipFileName);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetResourceType(CDelphi_WindowImplBase* handle ,UILIB_RESOURCETYPE RType) {
    handle->SetResourceType(RType);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetInitWindow(CDelphi_WindowImplBase* handle ,InitWindowCallBack Callback) {
    handle->SetInitWindow(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetFinalMessage(CDelphi_WindowImplBase* handle ,FinalMessageCallBack Callback) {
    handle->SetFinalMessage(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetHandleMessage(CDelphi_WindowImplBase* handle ,HandleMessageCallBack Callback) {
    handle->SetHandleMessage(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetNotify(CDelphi_WindowImplBase* handle ,NotifyCallBack Callback) {
    handle->SetNotify(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetClick(CDelphi_WindowImplBase* handle ,NotifyCallBack Callback) {
    handle->SetClick(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetMessageHandler(CDelphi_WindowImplBase* handle ,MessageCallBack Callback) {
    handle->SetMessageHandler(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetHandleCustomMessage(CDelphi_WindowImplBase* handle ,MessageCallBack Callback) {
    handle->SetHandleCustomMessage(Callback);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetCreateControl(CDelphi_WindowImplBase* handle ,CreateControlCallBack CallBack) {
    handle->SetCreateControl(CallBack);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetGetItemText(CDelphi_WindowImplBase* handle, GetItemTextCallBack ACallBack) {
	handle->SetGetItemText(ACallBack);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_SetGetClassStyle(CDelphi_WindowImplBase* handle, UINT uStyle) {
	handle->SetGetClassStyle(uStyle);
}

DRIECTUILIB_API void Delphi_Delphi_WindowImplBase_RemoveThisInPaintManager(CDelphi_WindowImplBase* handle) {
	handle->RemoveThisInPaintManager();
}

//================================CPaintManagerUI============================

DRIECTUILIB_API CPaintManagerUI* Delphi_PaintManagerUI_CppCreate() {
    return new CPaintManagerUI();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_CppDestroy(CPaintManagerUI* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_PaintManagerUI_Init(CPaintManagerUI* handle ,HWND hWnd) {
    handle->Init(hWnd);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_IsUpdateNeeded(CPaintManagerUI* handle) {
    return handle->IsUpdateNeeded();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_NeedUpdate(CPaintManagerUI* handle) {
    handle->NeedUpdate();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_Invalidate_01(CPaintManagerUI* handle) {
    handle->Invalidate();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_Invalidate_02(CPaintManagerUI* handle ,RECT& rcItem) {
    handle->Invalidate(rcItem);
}

DRIECTUILIB_API HDC Delphi_PaintManagerUI_GetPaintDC(CPaintManagerUI* handle) {
    return handle->GetPaintDC();
}

DRIECTUILIB_API HWND Delphi_PaintManagerUI_GetPaintWindow(CPaintManagerUI* handle) {
    return handle->GetPaintWindow();
}

DRIECTUILIB_API HWND Delphi_PaintManagerUI_GetTooltipWindow(CPaintManagerUI* handle) {
    return handle->GetTooltipWindow();
}

DRIECTUILIB_API POINT Delphi_PaintManagerUI_GetMousePos(CPaintManagerUI* handle) {
    return handle->GetMousePos();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_GetClientSize(CPaintManagerUI* handle, SIZE& Result) {
    Result = handle->GetClientSize();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_GetInitSize(CPaintManagerUI* handle, SIZE& Result) {
    Result = handle->GetInitSize();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetInitSize(CPaintManagerUI* handle ,int cx, int cy) {
    handle->SetInitSize(cx, cy);
}

DRIECTUILIB_API RECT& Delphi_PaintManagerUI_GetSizeBox(CPaintManagerUI* handle) {
    return handle->GetSizeBox();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetSizeBox(CPaintManagerUI* handle ,RECT& rcSizeBox) {
    handle->SetSizeBox(rcSizeBox);
}

DRIECTUILIB_API RECT& Delphi_PaintManagerUI_GetCaptionRect(CPaintManagerUI* handle) {
    return handle->GetCaptionRect();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetCaptionRect(CPaintManagerUI* handle ,RECT& rcCaption) {
    handle->SetCaptionRect(rcCaption);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_GetRoundCorner(CPaintManagerUI* handle, SIZE& Result) {
    Result = handle->GetRoundCorner();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetRoundCorner(CPaintManagerUI* handle ,int cx, int cy) {
    handle->SetRoundCorner(cx, cy);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_GetMinInfo(CPaintManagerUI* handle, SIZE& Result) {
    Result = handle->GetMinInfo();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetMinInfo(CPaintManagerUI* handle ,int cx, int cy) {
    handle->SetMinInfo(cx, cy);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_GetMaxInfo(CPaintManagerUI* handle, SIZE& Result) {
    Result = handle->GetMaxInfo();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetMaxInfo(CPaintManagerUI* handle ,int cx, int cy) {
    handle->SetMaxInfo(cx, cy);
}

DRIECTUILIB_API int Delphi_PaintManagerUI_GetTransparent(CPaintManagerUI* handle) {
    return handle->GetTransparent();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetTransparent(CPaintManagerUI* handle ,int nOpacity) {
    handle->SetTransparent(nOpacity);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetBackgroundTransparent(CPaintManagerUI* handle ,bool bTrans) {
    handle->SetBackgroundTransparent(bTrans);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_IsShowUpdateRect(CPaintManagerUI* handle) {
    return handle->IsShowUpdateRect();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetShowUpdateRect(CPaintManagerUI* handle ,bool show) {
    handle->SetShowUpdateRect(show);
}

DRIECTUILIB_API HINSTANCE Delphi_PaintManagerUI_GetInstance() {
    return CPaintManagerUI::GetInstance();
}

DRIECTUILIB_API CDuiString Delphi_PaintManagerUI_GetInstancePath() {
    return CPaintManagerUI::GetInstancePath();
}

DRIECTUILIB_API CDuiString Delphi_PaintManagerUI_GetCurrentPath() {
    return CPaintManagerUI::GetCurrentPath();
}

DRIECTUILIB_API HINSTANCE Delphi_PaintManagerUI_GetResourceDll() {
    return CPaintManagerUI::GetResourceDll();
}

DRIECTUILIB_API const CDuiString& Delphi_PaintManagerUI_GetResourcePath() {
    return CPaintManagerUI::GetResourcePath();
}

DRIECTUILIB_API const CDuiString& Delphi_PaintManagerUI_GetResourceZip() {
    return CPaintManagerUI::GetResourceZip();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_IsCachedResourceZip() {
    return CPaintManagerUI::IsCachedResourceZip();
}

DRIECTUILIB_API HANDLE Delphi_PaintManagerUI_GetResourceZipHandle() {
    return CPaintManagerUI::GetResourceZipHandle();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetInstance(HINSTANCE hInst) {
    CPaintManagerUI::SetInstance(hInst);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetCurrentPath(LPCTSTR pStrPath) {
    CPaintManagerUI::SetCurrentPath(pStrPath);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetResourceDll(HINSTANCE hInst) {
    CPaintManagerUI::SetResourceDll(hInst);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetResourcePath(LPCTSTR pStrPath) {
    CPaintManagerUI::SetResourcePath(pStrPath);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetResourceZip_01(LPVOID pVoid, unsigned int len) {
    CPaintManagerUI::SetResourceZip(pVoid, len);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetResourceZip_02(LPCTSTR pstrZip, bool bCachedResourceZip) {
    CPaintManagerUI::SetResourceZip(pstrZip, bCachedResourceZip);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_GetHSL(short* H, short* S, short* L) {
    return CPaintManagerUI::GetHSL(H, S, L);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ReloadSkin() {
    CPaintManagerUI::ReloadSkin();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_LoadPlugin(LPCTSTR pstrModuleName) {
    return CPaintManagerUI::LoadPlugin(pstrModuleName);
}

DRIECTUILIB_API CStdPtrArray* Delphi_PaintManagerUI_GetPlugins() {
    return CPaintManagerUI::GetPlugins();
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetDefaultDisabledColor(CPaintManagerUI* handle) {
    return handle->GetDefaultDisabledColor();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultDisabledColor(CPaintManagerUI* handle ,DWORD dwColor, bool bShared) {
    handle->SetDefaultDisabledColor(dwColor, bShared);
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetDefaultFontColor(CPaintManagerUI* handle) {
    return handle->GetDefaultFontColor();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultFontColor(CPaintManagerUI* handle ,DWORD dwColor, bool bShared) {
    handle->SetDefaultFontColor(dwColor, bShared);
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetDefaultLinkFontColor(CPaintManagerUI* handle) {
    return handle->GetDefaultLinkFontColor();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultLinkFontColor(CPaintManagerUI* handle ,DWORD dwColor, bool bShared) {
    handle->SetDefaultLinkFontColor(dwColor, bShared);
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetDefaultLinkHoverFontColor(CPaintManagerUI* handle) {
    return handle->GetDefaultLinkHoverFontColor();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultLinkHoverFontColor(CPaintManagerUI* handle ,DWORD dwColor, bool bShared) {
    handle->SetDefaultLinkHoverFontColor(dwColor, bShared);
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetDefaultSelectedBkColor(CPaintManagerUI* handle) {
    return handle->GetDefaultSelectedBkColor();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultSelectedBkColor(CPaintManagerUI* handle ,DWORD dwColor, bool bShared) {
    handle->SetDefaultSelectedBkColor(dwColor, bShared);
}

DRIECTUILIB_API TFontInfo* Delphi_PaintManagerUI_GetDefaultFontInfo(CPaintManagerUI* handle) {
    return handle->GetDefaultFontInfo();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetDefaultFont(CPaintManagerUI* handle ,LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared) {
    handle->SetDefaultFont(pStrFontName, nSize, bBold, bUnderline, bItalic, bShared);
}

DRIECTUILIB_API DWORD Delphi_PaintManagerUI_GetCustomFontCount(CPaintManagerUI* handle ,bool bShared) {
    return handle->GetCustomFontCount(bShared);
}

DRIECTUILIB_API HFONT Delphi_PaintManagerUI_AddFont(CPaintManagerUI* handle ,int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared) {
    return handle->AddFont(id, pStrFontName, nSize, bBold, bUnderline, bItalic, bShared);
}

DRIECTUILIB_API HFONT Delphi_PaintManagerUI_GetFont_01(CPaintManagerUI* handle ,int id) {
    return handle->GetFont(id);
}

DRIECTUILIB_API HFONT Delphi_PaintManagerUI_GetFont_02(CPaintManagerUI* handle ,LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic) {
    return handle->GetFont(pStrFontName, nSize, bBold, bUnderline, bItalic);
}

DRIECTUILIB_API int Delphi_PaintManagerUI_GetFontIndex_01(CPaintManagerUI* handle ,HFONT hFont, bool bShared) {
    return handle->GetFontIndex(hFont, bShared);
}

DRIECTUILIB_API int Delphi_PaintManagerUI_GetFontIndex_02(CPaintManagerUI* handle ,LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared) {
    return handle->GetFontIndex(pStrFontName, nSize, bBold, bUnderline, bItalic, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveFont_01(CPaintManagerUI* handle ,HFONT hFont, bool bShared) {
    handle->RemoveFont(hFont, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveFont_02(CPaintManagerUI* handle ,int id, bool bShared) {
    handle->RemoveFont(id, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllFonts(CPaintManagerUI* handle ,bool bShared) {
    handle->RemoveAllFonts(bShared);
}

DRIECTUILIB_API TFontInfo* Delphi_PaintManagerUI_GetFontInfo_01(CPaintManagerUI* handle ,int id) {
    return handle->GetFontInfo(id);
}

DRIECTUILIB_API TFontInfo* Delphi_PaintManagerUI_GetFontInfo_02(CPaintManagerUI* handle ,HFONT hFont) {
    return handle->GetFontInfo(hFont);
}

DRIECTUILIB_API const TImageInfo* Delphi_PaintManagerUI_GetImage(CPaintManagerUI* handle ,LPCTSTR bitmap) {
    return handle->GetImage(bitmap);
}

DRIECTUILIB_API const TImageInfo* Delphi_PaintManagerUI_GetImageEx(CPaintManagerUI* handle ,LPCTSTR bitmap, LPCTSTR type, DWORD mask, bool bUseHSL) {
    return handle->GetImageEx(bitmap, type, mask, bUseHSL);
}

DRIECTUILIB_API const TImageInfo* Delphi_PaintManagerUI_AddImage_01(CPaintManagerUI* handle ,LPCTSTR bitmap, LPCTSTR type, DWORD mask, bool bUseHSL, bool bShared) {
    return handle->AddImage(bitmap, type, mask, bUseHSL, bShared);
}

DRIECTUILIB_API const TImageInfo* Delphi_PaintManagerUI_AddImage_02(CPaintManagerUI* handle ,LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha, bool bShared) {
    return handle->AddImage(bitmap, hBitmap, iWidth, iHeight, bAlpha, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveImage(CPaintManagerUI* handle ,LPCTSTR bitmap, bool bShared) {
    handle->RemoveImage(bitmap, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllImages(CPaintManagerUI* handle ,bool bShared) {
    handle->RemoveAllImages(bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ReloadSharedImages() {
    CPaintManagerUI::ReloadSharedImages();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ReloadImages(CPaintManagerUI* handle) {
    handle->ReloadImages();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_AddDefaultAttributeList(CPaintManagerUI* handle ,LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared) {
    handle->AddDefaultAttributeList(pStrControlName, pStrControlAttrList, bShared);
}

DRIECTUILIB_API LPCTSTR Delphi_PaintManagerUI_GetDefaultAttributeList(CPaintManagerUI* handle ,LPCTSTR pStrControlName) {
    return handle->GetDefaultAttributeList(pStrControlName);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemoveDefaultAttributeList(CPaintManagerUI* handle ,LPCTSTR pStrControlName, bool bShared) {
    return handle->RemoveDefaultAttributeList(pStrControlName, bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllDefaultAttributeList(CPaintManagerUI* handle ,bool bShared) {
    handle->RemoveAllDefaultAttributeList(bShared);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_AddMultiLanguageString(int id, LPCTSTR pStrMultiLanguage) {
    CPaintManagerUI::AddMultiLanguageString(id, pStrMultiLanguage);
}

DRIECTUILIB_API LPCTSTR Delphi_PaintManagerUI_GetMultiLanguageString(int id) {
    return CPaintManagerUI::GetMultiLanguageString(id);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemoveMultiLanguageString(int id) {
    return CPaintManagerUI::RemoveMultiLanguageString(id);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllMultiLanguageString() {
    CPaintManagerUI::RemoveAllMultiLanguageString();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ProcessMultiLanguageTokens(CDuiString& pStrMultiLanguage) {
    CPaintManagerUI::ProcessMultiLanguageTokens(pStrMultiLanguage);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AttachDialog(CPaintManagerUI* handle ,CControlUI* pControl) {
    return handle->AttachDialog(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_InitControls(CPaintManagerUI* handle ,CControlUI* pControl, CControlUI* pParent) {
    return handle->InitControls(pControl, pParent);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ReapObjects(CPaintManagerUI* handle ,CControlUI* pControl) {
    handle->ReapObjects(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddOptionGroup(CPaintManagerUI* handle ,LPCTSTR pStrGroupName, CControlUI* pControl) {
    return handle->AddOptionGroup(pStrGroupName, pControl);
}

DRIECTUILIB_API CStdPtrArray* Delphi_PaintManagerUI_GetOptionGroup(CPaintManagerUI* handle ,LPCTSTR pStrGroupName) {
    return handle->GetOptionGroup(pStrGroupName);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveOptionGroup(CPaintManagerUI* handle ,LPCTSTR pStrGroupName, CControlUI* pControl) {
    handle->RemoveOptionGroup(pStrGroupName, pControl);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllOptionGroups(CPaintManagerUI* handle) {
    handle->RemoveAllOptionGroups();
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_GetFocus(CPaintManagerUI* handle) {
    return handle->GetFocus();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetFocus(CPaintManagerUI* handle ,CControlUI* pControl) {
    handle->SetFocus(pControl);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetFocusNeeded(CPaintManagerUI* handle ,CControlUI* pControl) {
    handle->SetFocusNeeded(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_SetNextTabControl(CPaintManagerUI* handle ,bool bForward) {
    return handle->SetNextTabControl(bForward);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_SetTimer(CPaintManagerUI* handle ,CControlUI* pControl, UINT nTimerID, UINT uElapse) {
    return handle->SetTimer(pControl, nTimerID, uElapse);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_KillTimer_01(CPaintManagerUI* handle ,CControlUI* pControl, UINT nTimerID) {
    return handle->KillTimer(pControl, nTimerID);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_KillTimer_02(CPaintManagerUI* handle ,CControlUI* pControl) {
    handle->KillTimer(pControl);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_RemoveAllTimers(CPaintManagerUI* handle) {
    handle->RemoveAllTimers();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SetCapture(CPaintManagerUI* handle) {
    handle->SetCapture();
}

DRIECTUILIB_API void Delphi_PaintManagerUI_ReleaseCapture(CPaintManagerUI* handle) {
    handle->ReleaseCapture();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_IsCaptured(CPaintManagerUI* handle) {
    return handle->IsCaptured();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddNotifier(CPaintManagerUI* handle ,INotifyUI* pControl) {
    return handle->AddNotifier(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemoveNotifier(CPaintManagerUI* handle ,INotifyUI* pControl) {
    return handle->RemoveNotifier(pControl);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SendNotify_01(CPaintManagerUI* handle ,TNotifyUI& Msg, bool bAsync) {
    handle->SendNotify(Msg, bAsync);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_SendNotify_02(CPaintManagerUI* handle ,CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam, LPARAM lParam, bool bAsync) {
    handle->SendNotify(pControl, pstrMessage, wParam, lParam, bAsync);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddPreMessageFilter(CPaintManagerUI* handle ,IMessageFilterUI* pFilter) {
    return handle->AddPreMessageFilter(pFilter);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemovePreMessageFilter(CPaintManagerUI* handle ,IMessageFilterUI* pFilter) {
    return handle->RemovePreMessageFilter(pFilter);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddMessageFilter(CPaintManagerUI* handle ,IMessageFilterUI* pFilter) {
    return handle->AddMessageFilter(pFilter);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemoveMessageFilter(CPaintManagerUI* handle ,IMessageFilterUI* pFilter) {
    return handle->RemoveMessageFilter(pFilter);
}

DRIECTUILIB_API int Delphi_PaintManagerUI_GetPostPaintCount(CPaintManagerUI* handle) {
    return handle->GetPostPaintCount();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddPostPaint(CPaintManagerUI* handle ,CControlUI* pControl) {
    return handle->AddPostPaint(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemovePostPaint(CPaintManagerUI* handle ,CControlUI* pControl) {
    return handle->RemovePostPaint(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_SetPostPaintIndex(CPaintManagerUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->SetPostPaintIndex(pControl, iIndex);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_AddDelayedCleanup(CPaintManagerUI* handle ,CControlUI* pControl) {
    handle->AddDelayedCleanup(pControl);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_AddTranslateAccelerator(CPaintManagerUI* handle ,ITranslateAccelerator* pTranslateAccelerator) {
    return handle->AddTranslateAccelerator(pTranslateAccelerator);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_RemoveTranslateAccelerator(CPaintManagerUI* handle ,ITranslateAccelerator* pTranslateAccelerator) {
    return handle->RemoveTranslateAccelerator(pTranslateAccelerator);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_TranslateAccelerator(CPaintManagerUI* handle ,LPMSG pMsg) {
    return handle->TranslateAccelerator(pMsg);
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_GetRoot(CPaintManagerUI* handle) {
    return handle->GetRoot();
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_FindControl_01(CPaintManagerUI* handle ,POINT pt) {
    return handle->FindControl(pt);
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_FindControl_02(CPaintManagerUI* handle ,LPCTSTR pstrName) {
    return handle->FindControl(pstrName);
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_FindSubControlByPoint(CPaintManagerUI* handle ,CControlUI* pParent, POINT pt) {
    return handle->FindSubControlByPoint(pParent, pt);
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_FindSubControlByName(CPaintManagerUI* handle ,CControlUI* pParent, LPCTSTR pstrName) {
    return handle->FindSubControlByName(pParent, pstrName);
}

DRIECTUILIB_API CControlUI* Delphi_PaintManagerUI_FindSubControlByClass(CPaintManagerUI* handle ,CControlUI* pParent, LPCTSTR pstrClass, int iIndex) {
    return handle->FindSubControlByClass(pParent, pstrClass, iIndex);
}

DRIECTUILIB_API CStdPtrArray* Delphi_PaintManagerUI_FindSubControlsByClass(CPaintManagerUI* handle ,CControlUI* pParent, LPCTSTR pstrClass) {
    return handle->FindSubControlsByClass(pParent, pstrClass);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_MessageLoop() {
    CPaintManagerUI::MessageLoop();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_TranslateMessage(LPMSG pMsg) {
    return CPaintManagerUI::TranslateMessage(pMsg);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_Term() {
    CPaintManagerUI::Term();
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_MessageHandler(CPaintManagerUI* handle ,UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes) {
    return handle->MessageHandler(uMsg, wParam, lParam, lRes);
}

DRIECTUILIB_API bool Delphi_PaintManagerUI_PreMessageHandler(CPaintManagerUI* handle ,UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes) {
    return handle->PreMessageHandler(uMsg, wParam, lParam, lRes);
}

DRIECTUILIB_API void Delphi_PaintManagerUI_UsedVirtualWnd(CPaintManagerUI* handle ,bool bUsed) {
    handle->UsedVirtualWnd(bUsed);
}

//================================CContainerUI============================

DRIECTUILIB_API CContainerUI* Delphi_ContainerUI_CppCreate() {
    return new CContainerUI();
}

DRIECTUILIB_API void Delphi_ContainerUI_CppDestroy(CContainerUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ContainerUI_GetClass(CContainerUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ContainerUI_GetInterface(CContainerUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API CControlUI* Delphi_ContainerUI_GetItemAt(CContainerUI* handle ,int iIndex) {
    return handle->GetItemAt(iIndex);
}

DRIECTUILIB_API int Delphi_ContainerUI_GetItemIndex(CContainerUI* handle ,CControlUI* pControl) {
    return handle->GetItemIndex(pControl);
}

DRIECTUILIB_API bool Delphi_ContainerUI_SetItemIndex(CContainerUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->SetItemIndex(pControl, iIndex);
}

DRIECTUILIB_API int Delphi_ContainerUI_GetCount(CContainerUI* handle) {
    return handle->GetCount();
}

DRIECTUILIB_API bool Delphi_ContainerUI_Add(CContainerUI* handle ,CControlUI* pControl) {
    return handle->Add(pControl);
}

DRIECTUILIB_API bool Delphi_ContainerUI_AddAt(CContainerUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_ContainerUI_Remove(CContainerUI* handle ,CControlUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API bool Delphi_ContainerUI_RemoveAt(CContainerUI* handle ,int iIndex) {
    return handle->RemoveAt(iIndex);
}

DRIECTUILIB_API void Delphi_ContainerUI_RemoveAll(CContainerUI* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API void Delphi_ContainerUI_DoEvent(CContainerUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetVisible(CContainerUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetInternVisible(CContainerUI* handle ,bool bVisible) {
    handle->SetInternVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetMouseEnabled(CContainerUI* handle ,bool bEnable) {
    handle->SetMouseEnabled(bEnable);
}

DRIECTUILIB_API void Delphi_ContainerUI_GetInset(CContainerUI* handle, RECT& Result) {
    Result = handle->GetInset();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetInset(CContainerUI* handle ,RECT rcInset) {
    handle->SetInset(rcInset);
}

DRIECTUILIB_API int Delphi_ContainerUI_GetChildPadding(CContainerUI* handle) {
    return handle->GetChildPadding();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetChildPadding(CContainerUI* handle ,int iPadding) {
    handle->SetChildPadding(iPadding);
}

DRIECTUILIB_API bool Delphi_ContainerUI_IsAutoDestroy(CContainerUI* handle) {
    return handle->IsAutoDestroy();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetAutoDestroy(CContainerUI* handle ,bool bAuto) {
    handle->SetAutoDestroy(bAuto);
}

DRIECTUILIB_API bool Delphi_ContainerUI_IsDelayedDestroy(CContainerUI* handle) {
    return handle->IsDelayedDestroy();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetDelayedDestroy(CContainerUI* handle ,bool bDelayed) {
    handle->SetDelayedDestroy(bDelayed);
}

DRIECTUILIB_API bool Delphi_ContainerUI_IsMouseChildEnabled(CContainerUI* handle) {
    return handle->IsMouseChildEnabled();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetMouseChildEnabled(CContainerUI* handle ,bool bEnable) {
    handle->SetMouseChildEnabled(bEnable);
}

DRIECTUILIB_API int Delphi_ContainerUI_FindSelectable(CContainerUI* handle ,int iIndex, bool bForward) {
    return handle->FindSelectable(iIndex, bForward);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetPos(CContainerUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ContainerUI_Move(CContainerUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ContainerUI_DoPaint(CContainerUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetAttribute(CContainerUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ContainerUI_SetManager(CContainerUI* handle ,CPaintManagerUI* pManager, CControlUI* pParent, bool bInit) {
    handle->SetManager(pManager, pParent, bInit);
}

DRIECTUILIB_API CControlUI* Delphi_ContainerUI_FindControl(CContainerUI* handle ,FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags) {
    return handle->FindControl(Proc, pData, uFlags);
}

DRIECTUILIB_API bool Delphi_ContainerUI_SetSubControlText(CContainerUI* handle ,LPCTSTR pstrSubControlName, LPCTSTR pstrText) {
    return handle->SetSubControlText(pstrSubControlName, pstrText);
}

DRIECTUILIB_API bool Delphi_ContainerUI_SetSubControlFixedHeight(CContainerUI* handle ,LPCTSTR pstrSubControlName, int cy) {
    return handle->SetSubControlFixedHeight(pstrSubControlName, cy);
}

DRIECTUILIB_API bool Delphi_ContainerUI_SetSubControlFixedWdith(CContainerUI* handle ,LPCTSTR pstrSubControlName, int cx) {
    return handle->SetSubControlFixedWdith(pstrSubControlName, cx);
}

DRIECTUILIB_API bool Delphi_ContainerUI_SetSubControlUserData(CContainerUI* handle ,LPCTSTR pstrSubControlName, LPCTSTR pstrText) {
    return handle->SetSubControlUserData(pstrSubControlName, pstrText);
}

DRIECTUILIB_API CDuiString Delphi_ContainerUI_GetSubControlText(CContainerUI* handle ,LPCTSTR pstrSubControlName) {
    return handle->GetSubControlText(pstrSubControlName);
}

DRIECTUILIB_API int Delphi_ContainerUI_GetSubControlFixedHeight(CContainerUI* handle ,LPCTSTR pstrSubControlName) {
    return handle->GetSubControlFixedHeight(pstrSubControlName);
}

DRIECTUILIB_API int Delphi_ContainerUI_GetSubControlFixedWdith(CContainerUI* handle ,LPCTSTR pstrSubControlName) {
    return handle->GetSubControlFixedWdith(pstrSubControlName);
}

DRIECTUILIB_API const CDuiString Delphi_ContainerUI_GetSubControlUserData(CContainerUI* handle ,LPCTSTR pstrSubControlName) {
    return handle->GetSubControlUserData(pstrSubControlName);
}

DRIECTUILIB_API CControlUI* Delphi_ContainerUI_FindSubControl(CContainerUI* handle ,LPCTSTR pstrSubControlName) {
    return handle->FindSubControl(pstrSubControlName);
}

DRIECTUILIB_API void Delphi_ContainerUI_GetScrollPos(CContainerUI* handle, SIZE& Result) {
    Result = handle->GetScrollPos();
}

DRIECTUILIB_API void Delphi_ContainerUI_GetScrollRange(CContainerUI* handle, SIZE& Result) {
    Result = handle->GetScrollRange();
}

DRIECTUILIB_API void Delphi_ContainerUI_SetScrollPos(CContainerUI* handle ,SIZE szPos) {
    handle->SetScrollPos(szPos);
}

DRIECTUILIB_API void Delphi_ContainerUI_LineUp(CContainerUI* handle) {
    handle->LineUp();
}

DRIECTUILIB_API void Delphi_ContainerUI_LineDown(CContainerUI* handle) {
    handle->LineDown();
}

DRIECTUILIB_API void Delphi_ContainerUI_PageUp(CContainerUI* handle) {
    handle->PageUp();
}

DRIECTUILIB_API void Delphi_ContainerUI_PageDown(CContainerUI* handle) {
    handle->PageDown();
}

DRIECTUILIB_API void Delphi_ContainerUI_HomeUp(CContainerUI* handle) {
    handle->HomeUp();
}

DRIECTUILIB_API void Delphi_ContainerUI_EndDown(CContainerUI* handle) {
    handle->EndDown();
}

DRIECTUILIB_API void Delphi_ContainerUI_LineLeft(CContainerUI* handle) {
    handle->LineLeft();
}

DRIECTUILIB_API void Delphi_ContainerUI_LineRight(CContainerUI* handle) {
    handle->LineRight();
}

DRIECTUILIB_API void Delphi_ContainerUI_PageLeft(CContainerUI* handle) {
    handle->PageLeft();
}

DRIECTUILIB_API void Delphi_ContainerUI_PageRight(CContainerUI* handle) {
    handle->PageRight();
}

DRIECTUILIB_API void Delphi_ContainerUI_HomeLeft(CContainerUI* handle) {
    handle->HomeLeft();
}

DRIECTUILIB_API void Delphi_ContainerUI_EndRight(CContainerUI* handle) {
    handle->EndRight();
}

DRIECTUILIB_API void Delphi_ContainerUI_EnableScrollBar(CContainerUI* handle ,bool bEnableVertical, bool bEnableHorizontal) {
    handle->EnableScrollBar(bEnableVertical, bEnableHorizontal);
}

DRIECTUILIB_API CScrollBarUI* Delphi_ContainerUI_GetVerticalScrollBar(CContainerUI* handle) {
    return handle->GetVerticalScrollBar();
}

DRIECTUILIB_API CScrollBarUI* Delphi_ContainerUI_GetHorizontalScrollBar(CContainerUI* handle) {
    return handle->GetHorizontalScrollBar();
}

//================================CVerticalLayoutUI============================

DRIECTUILIB_API CVerticalLayoutUI* Delphi_VerticalLayoutUI_CppCreate() {
    return new CVerticalLayoutUI();
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_CppDestroy(CVerticalLayoutUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_VerticalLayoutUI_GetClass(CVerticalLayoutUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_VerticalLayoutUI_GetInterface(CVerticalLayoutUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_VerticalLayoutUI_GetControlFlags(CVerticalLayoutUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_SetSepHeight(CVerticalLayoutUI* handle ,int iHeight) {
    handle->SetSepHeight(iHeight);
}

DRIECTUILIB_API int Delphi_VerticalLayoutUI_GetSepHeight(CVerticalLayoutUI* handle) {
    return handle->GetSepHeight();
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_SetSepImmMode(CVerticalLayoutUI* handle ,bool bImmediately) {
    handle->SetSepImmMode(bImmediately);
}

DRIECTUILIB_API bool Delphi_VerticalLayoutUI_IsSepImmMode(CVerticalLayoutUI* handle) {
    return handle->IsSepImmMode();
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_SetAttribute(CVerticalLayoutUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_DoEvent(CVerticalLayoutUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_SetPos(CVerticalLayoutUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_DoPostPaint(CVerticalLayoutUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPostPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_VerticalLayoutUI_GetThumbRect(CVerticalLayoutUI* handle ,bool bUseNew, RECT& Result) {
    Result = handle->GetThumbRect(bUseNew);
}

//================================CListUI============================

DRIECTUILIB_API CListUI* Delphi_ListUI_CppCreate() {
    return new CListUI();
}

DRIECTUILIB_API void Delphi_ListUI_CppDestroy(CListUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ListUI_GetClass(CListUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API UINT Delphi_ListUI_GetControlFlags(CListUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API LPVOID Delphi_ListUI_GetInterface(CListUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API bool Delphi_ListUI_GetScrollSelect(CListUI* handle) {
    return handle->GetScrollSelect();
}

DRIECTUILIB_API void Delphi_ListUI_SetScrollSelect(CListUI* handle ,bool bScrollSelect) {
    handle->SetScrollSelect(bScrollSelect);
}

DRIECTUILIB_API int Delphi_ListUI_GetCurSel(CListUI* handle) {
    return handle->GetCurSel();
}

DRIECTUILIB_API bool Delphi_ListUI_SelectItem(CListUI* handle ,int iIndex, bool bTakeFocus) {
    return handle->SelectItem(iIndex, bTakeFocus);
}

DRIECTUILIB_API CListHeaderUI* Delphi_ListUI_GetHeader(CListUI* handle) {
    return handle->GetHeader();
}

DRIECTUILIB_API CContainerUI* Delphi_ListUI_GetList(CListUI* handle) {
    return handle->GetList();
}

DRIECTUILIB_API TListInfoUI* Delphi_ListUI_GetListInfo(CListUI* handle) {
    return handle->GetListInfo();
}

DRIECTUILIB_API CControlUI* Delphi_ListUI_GetItemAt(CListUI* handle ,int iIndex) {
    return handle->GetItemAt(iIndex);
}

DRIECTUILIB_API int Delphi_ListUI_GetItemIndex(CListUI* handle ,CControlUI* pControl) {
    return handle->GetItemIndex(pControl);
}

DRIECTUILIB_API bool Delphi_ListUI_SetItemIndex(CListUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->SetItemIndex(pControl, iIndex);
}

DRIECTUILIB_API int Delphi_ListUI_GetCount(CListUI* handle) {
    return handle->GetCount();
}

DRIECTUILIB_API bool Delphi_ListUI_Add(CListUI* handle ,CControlUI* pControl) {
    return handle->Add(pControl);
}

DRIECTUILIB_API bool Delphi_ListUI_AddAt(CListUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_ListUI_Remove(CListUI* handle ,CControlUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API bool Delphi_ListUI_RemoveAt(CListUI* handle ,int iIndex) {
    return handle->RemoveAt(iIndex);
}

DRIECTUILIB_API void Delphi_ListUI_RemoveAll(CListUI* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API void Delphi_ListUI_EnsureVisible(CListUI* handle ,int iIndex) {
    handle->EnsureVisible(iIndex);
}

DRIECTUILIB_API void Delphi_ListUI_Scroll(CListUI* handle ,int dx, int dy) {
    handle->Scroll(dx, dy);
}

DRIECTUILIB_API int Delphi_ListUI_GetChildPadding(CListUI* handle) {
    return handle->GetChildPadding();
}

DRIECTUILIB_API void Delphi_ListUI_SetChildPadding(CListUI* handle ,int iPadding) {
    handle->SetChildPadding(iPadding);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemFont(CListUI* handle ,int index) {
    handle->SetItemFont(index);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemTextStyle(CListUI* handle ,UINT uStyle) {
    handle->SetItemTextStyle(uStyle);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemTextPadding(CListUI* handle ,RECT rc) {
    handle->SetItemTextPadding(rc);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemTextColor(CListUI* handle ,DWORD dwTextColor) {
    handle->SetItemTextColor(dwTextColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemBkColor(CListUI* handle ,DWORD dwBkColor) {
    handle->SetItemBkColor(dwBkColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemBkImage(CListUI* handle ,LPCTSTR pStrImage) {
    handle->SetItemBkImage(pStrImage);
}

DRIECTUILIB_API bool Delphi_ListUI_IsAlternateBk(CListUI* handle) {
    return handle->IsAlternateBk();
}

DRIECTUILIB_API void Delphi_ListUI_SetAlternateBk(CListUI* handle ,bool bAlternateBk) {
    handle->SetAlternateBk(bAlternateBk);
}

DRIECTUILIB_API void Delphi_ListUI_SetSelectedItemTextColor(CListUI* handle ,DWORD dwTextColor) {
    handle->SetSelectedItemTextColor(dwTextColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetSelectedItemBkColor(CListUI* handle ,DWORD dwBkColor) {
    handle->SetSelectedItemBkColor(dwBkColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetSelectedItemImage(CListUI* handle ,LPCTSTR pStrImage) {
    handle->SetSelectedItemImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ListUI_SetHotItemTextColor(CListUI* handle ,DWORD dwTextColor) {
    handle->SetHotItemTextColor(dwTextColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetHotItemBkColor(CListUI* handle ,DWORD dwBkColor) {
    handle->SetHotItemBkColor(dwBkColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetHotItemImage(CListUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotItemImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ListUI_SetDisabledItemTextColor(CListUI* handle ,DWORD dwTextColor) {
    handle->SetDisabledItemTextColor(dwTextColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetDisabledItemBkColor(CListUI* handle ,DWORD dwBkColor) {
    handle->SetDisabledItemBkColor(dwBkColor);
}

DRIECTUILIB_API void Delphi_ListUI_SetDisabledItemImage(CListUI* handle ,LPCTSTR pStrImage) {
    handle->SetDisabledItemImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ListUI_SetItemLineColor(CListUI* handle ,DWORD dwLineColor) {
    handle->SetItemLineColor(dwLineColor);
}

DRIECTUILIB_API bool Delphi_ListUI_IsItemShowHtml(CListUI* handle) {
    return handle->IsItemShowHtml();
}

DRIECTUILIB_API void Delphi_ListUI_SetItemShowHtml(CListUI* handle ,bool bShowHtml) {
    handle->SetItemShowHtml(bShowHtml);
}

DRIECTUILIB_API void Delphi_ListUI_GetItemTextPadding(CListUI* handle, RECT& Result) {
    Result = handle->GetItemTextPadding();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetItemTextColor(CListUI* handle) {
    return handle->GetItemTextColor();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetItemBkColor(CListUI* handle) {
    return handle->GetItemBkColor();
}

DRIECTUILIB_API LPCTSTR Delphi_ListUI_GetItemBkImage(CListUI* handle) {
    return handle->GetItemBkImage();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetSelectedItemTextColor(CListUI* handle) {
    return handle->GetSelectedItemTextColor();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetSelectedItemBkColor(CListUI* handle) {
    return handle->GetSelectedItemBkColor();
}

DRIECTUILIB_API LPCTSTR Delphi_ListUI_GetSelectedItemImage(CListUI* handle) {
    return handle->GetSelectedItemImage();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetHotItemTextColor(CListUI* handle) {
    return handle->GetHotItemTextColor();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetHotItemBkColor(CListUI* handle) {
    return handle->GetHotItemBkColor();
}

DRIECTUILIB_API LPCTSTR Delphi_ListUI_GetHotItemImage(CListUI* handle) {
    return handle->GetHotItemImage();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetDisabledItemTextColor(CListUI* handle) {
    return handle->GetDisabledItemTextColor();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetDisabledItemBkColor(CListUI* handle) {
    return handle->GetDisabledItemBkColor();
}

DRIECTUILIB_API LPCTSTR Delphi_ListUI_GetDisabledItemImage(CListUI* handle) {
    return handle->GetDisabledItemImage();
}

DRIECTUILIB_API DWORD Delphi_ListUI_GetItemLineColor(CListUI* handle) {
    return handle->GetItemLineColor();
}

DRIECTUILIB_API void Delphi_ListUI_SetMultiExpanding(CListUI* handle ,bool bMultiExpandable) {
    handle->SetMultiExpanding(bMultiExpandable);
}

DRIECTUILIB_API int Delphi_ListUI_GetExpandedItem(CListUI* handle) {
    return handle->GetExpandedItem();
}

DRIECTUILIB_API bool Delphi_ListUI_ExpandItem(CListUI* handle ,int iIndex, bool bExpand) {
    return handle->ExpandItem(iIndex, bExpand);
}

DRIECTUILIB_API void Delphi_ListUI_SetPos(CListUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ListUI_Move(CListUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ListUI_DoEvent(CListUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ListUI_SetAttribute(CListUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API IListCallbackUI* Delphi_ListUI_GetTextCallback(CListUI* handle) {
    return handle->GetTextCallback();
}

DRIECTUILIB_API void Delphi_ListUI_SetTextCallback(CListUI* handle ,IListCallbackUI* pCallback) {
    handle->SetTextCallback(pCallback);
}

DRIECTUILIB_API void Delphi_ListUI_GetScrollPos(CListUI* handle, SIZE& Result) {
    Result = handle->GetScrollPos();
}

DRIECTUILIB_API void Delphi_ListUI_GetScrollRange(CListUI* handle, SIZE& Result) {
    Result = handle->GetScrollRange();
}

DRIECTUILIB_API void Delphi_ListUI_SetScrollPos(CListUI* handle ,SIZE szPos) {
    handle->SetScrollPos(szPos);
}

DRIECTUILIB_API void Delphi_ListUI_LineUp(CListUI* handle) {
    handle->LineUp();
}

DRIECTUILIB_API void Delphi_ListUI_LineDown(CListUI* handle) {
    handle->LineDown();
}

DRIECTUILIB_API void Delphi_ListUI_PageUp(CListUI* handle) {
    handle->PageUp();
}

DRIECTUILIB_API void Delphi_ListUI_PageDown(CListUI* handle) {
    handle->PageDown();
}

DRIECTUILIB_API void Delphi_ListUI_HomeUp(CListUI* handle) {
    handle->HomeUp();
}

DRIECTUILIB_API void Delphi_ListUI_EndDown(CListUI* handle) {
    handle->EndDown();
}

DRIECTUILIB_API void Delphi_ListUI_LineLeft(CListUI* handle) {
    handle->LineLeft();
}

DRIECTUILIB_API void Delphi_ListUI_LineRight(CListUI* handle) {
    handle->LineRight();
}

DRIECTUILIB_API void Delphi_ListUI_PageLeft(CListUI* handle) {
    handle->PageLeft();
}

DRIECTUILIB_API void Delphi_ListUI_PageRight(CListUI* handle) {
    handle->PageRight();
}

DRIECTUILIB_API void Delphi_ListUI_HomeLeft(CListUI* handle) {
    handle->HomeLeft();
}

DRIECTUILIB_API void Delphi_ListUI_EndRight(CListUI* handle) {
    handle->EndRight();
}

DRIECTUILIB_API void Delphi_ListUI_EnableScrollBar(CListUI* handle ,bool bEnableVertical, bool bEnableHorizontal) {
    handle->EnableScrollBar(bEnableVertical, bEnableHorizontal);
}

DRIECTUILIB_API CScrollBarUI* Delphi_ListUI_GetVerticalScrollBar(CListUI* handle) {
    return handle->GetVerticalScrollBar();
}

DRIECTUILIB_API CScrollBarUI* Delphi_ListUI_GetHorizontalScrollBar(CListUI* handle) {
    return handle->GetHorizontalScrollBar();
}

DRIECTUILIB_API BOOL Delphi_ListUI_SortItems(CListUI* handle ,PULVCompareFunc pfnCompare, UINT_PTR dwData) {
    return handle->SortItems(pfnCompare, dwData);
}

//================================CDelphi_ListUI============================

DRIECTUILIB_API CDelphi_ListUI* Delphi_Delphi_ListUI_CppCreate() {
    return new CDelphi_ListUI();
}

DRIECTUILIB_API void Delphi_Delphi_ListUI_CppDestroy(CDelphi_ListUI* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_Delphi_ListUI_SetDelphiSelf(CDelphi_ListUI* handle ,LPVOID ASelf) {
    handle->SetDelphiSelf(ASelf);
}

DRIECTUILIB_API void Delphi_Delphi_ListUI_SetDoEvent(CDelphi_ListUI* handle ,DoEventCallBack ADoEvent) {
    handle->SetDoEvent(ADoEvent);
}

//================================CLabelUI============================

DRIECTUILIB_API CLabelUI* Delphi_LabelUI_CppCreate() {
    return new CLabelUI();
}

DRIECTUILIB_API void Delphi_LabelUI_CppDestroy(CLabelUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_LabelUI_GetClass(CLabelUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_LabelUI_GetInterface(CLabelUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextStyle(CLabelUI* handle ,UINT uStyle) {
    handle->SetTextStyle(uStyle);
}

DRIECTUILIB_API UINT Delphi_LabelUI_GetTextStyle(CLabelUI* handle) {
    return handle->GetTextStyle();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextColor(CLabelUI* handle ,DWORD dwTextColor) {
    handle->SetTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetTextColor(CLabelUI* handle) {
    return handle->GetTextColor();
}

DRIECTUILIB_API void Delphi_LabelUI_SetDisabledTextColor(CLabelUI* handle ,DWORD dwTextColor) {
    handle->SetDisabledTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetDisabledTextColor(CLabelUI* handle) {
    return handle->GetDisabledTextColor();
}

DRIECTUILIB_API void Delphi_LabelUI_SetFont(CLabelUI* handle ,int index) {
    handle->SetFont(index);
}

DRIECTUILIB_API int Delphi_LabelUI_GetFont(CLabelUI* handle) {
    return handle->GetFont();
}

DRIECTUILIB_API void Delphi_LabelUI_GetTextPadding(CLabelUI* handle, RECT& Result) {
    Result = handle->GetTextPadding();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextPadding(CLabelUI* handle ,RECT rc) {
    handle->SetTextPadding(rc);
}

DRIECTUILIB_API bool Delphi_LabelUI_IsShowHtml(CLabelUI* handle) {
    return handle->IsShowHtml();
}

DRIECTUILIB_API void Delphi_LabelUI_SetShowHtml(CLabelUI* handle ,bool bShowHtml) {
    handle->SetShowHtml(bShowHtml);
}

DRIECTUILIB_API void Delphi_LabelUI_EstimateSize(CLabelUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_LabelUI_DoEvent(CLabelUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_LabelUI_SetAttribute(CLabelUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_LabelUI_PaintText(CLabelUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

DRIECTUILIB_API void Delphi_LabelUI_SetEnabledEffect(CLabelUI* handle ,bool _EnabledEffect) {
    handle->SetEnabledEffect(_EnabledEffect);
}

DRIECTUILIB_API bool Delphi_LabelUI_GetEnabledEffect(CLabelUI* handle) {
    return handle->GetEnabledEffect();
}

DRIECTUILIB_API void Delphi_LabelUI_SetText(CLabelUI* handle ,LPCTSTR pstrText) {
    handle->SetText(pstrText);
}

DRIECTUILIB_API CDuiString Delphi_LabelUI_GetText(CLabelUI* handle) {
    return handle->GetText();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTransShadow(CLabelUI* handle ,int _TransShadow) {
    handle->SetTransShadow(_TransShadow);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTransShadow(CLabelUI* handle) {
    return handle->GetTransShadow();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTransShadow1(CLabelUI* handle ,int _TransShadow) {
    handle->SetTransShadow1(_TransShadow);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTransShadow1(CLabelUI* handle) {
    return handle->GetTransShadow1();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTransText(CLabelUI* handle ,int _TransText) {
    handle->SetTransText(_TransText);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTransText(CLabelUI* handle) {
    return handle->GetTransText();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTransText1(CLabelUI* handle ,int _TransText) {
    handle->SetTransText1(_TransText);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTransText1(CLabelUI* handle) {
    return handle->GetTransText1();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTransStroke(CLabelUI* handle ,int _TransStroke) {
    handle->SetTransStroke(_TransStroke);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTransStroke(CLabelUI* handle) {
    return handle->GetTransStroke();
}

DRIECTUILIB_API void Delphi_LabelUI_SetGradientLength(CLabelUI* handle ,int _GradientLength) {
    handle->SetGradientLength(_GradientLength);
}

DRIECTUILIB_API int Delphi_LabelUI_GetGradientLength(CLabelUI* handle) {
    return handle->GetGradientLength();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextRenderingHintAntiAlias(CLabelUI* handle ,int _TextRenderingHintAntiAlias) {
    handle->SetTextRenderingHintAntiAlias(_TextRenderingHintAntiAlias);
}

DRIECTUILIB_API int Delphi_LabelUI_GetTextRenderingHintAntiAlias(CLabelUI* handle) {
    return handle->GetTextRenderingHintAntiAlias();
}

DRIECTUILIB_API void Delphi_LabelUI_SetShadowOffset(CLabelUI* handle ,int _offset, int _angle) {
    handle->SetShadowOffset(_offset, _angle);
}

DRIECTUILIB_API void Delphi_LabelUI_GetShadowOffset(CLabelUI* handle, RectF& Result) {
    Result = handle->GetShadowOffset();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextColor1(CLabelUI* handle ,DWORD _TextColor1) {
    handle->SetTextColor1(_TextColor1);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetTextColor1(CLabelUI* handle) {
    return handle->GetTextColor1();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextShadowColorA(CLabelUI* handle ,DWORD _TextShadowColorA) {
    handle->SetTextShadowColorA(_TextShadowColorA);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetTextShadowColorA(CLabelUI* handle) {
    return handle->GetTextShadowColorA();
}

DRIECTUILIB_API void Delphi_LabelUI_SetTextShadowColorB(CLabelUI* handle ,DWORD _TextShadowColorB) {
    handle->SetTextShadowColorB(_TextShadowColorB);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetTextShadowColorB(CLabelUI* handle) {
    return handle->GetTextShadowColorB();
}

DRIECTUILIB_API void Delphi_LabelUI_SetStrokeColor(CLabelUI* handle ,DWORD _StrokeColor) {
    handle->SetStrokeColor(_StrokeColor);
}

DRIECTUILIB_API DWORD Delphi_LabelUI_GetStrokeColor(CLabelUI* handle) {
    return handle->GetStrokeColor();
}

DRIECTUILIB_API void Delphi_LabelUI_SetGradientAngle(CLabelUI* handle ,int _SetGradientAngle) {
    handle->SetGradientAngle(_SetGradientAngle);
}

DRIECTUILIB_API int Delphi_LabelUI_GetGradientAngle(CLabelUI* handle) {
    return handle->GetGradientAngle();
}

DRIECTUILIB_API void Delphi_LabelUI_SetEnabledStroke(CLabelUI* handle ,bool _EnabledStroke) {
    handle->SetEnabledStroke(_EnabledStroke);
}

DRIECTUILIB_API bool Delphi_LabelUI_GetEnabledStroke(CLabelUI* handle) {
    return handle->GetEnabledStroke();
}

DRIECTUILIB_API void Delphi_LabelUI_SetEnabledShadow(CLabelUI* handle ,bool _EnabledShadowe) {
    handle->SetEnabledShadow(_EnabledShadowe);
}

DRIECTUILIB_API bool Delphi_LabelUI_GetEnabledShadow(CLabelUI* handle) {
    return handle->GetEnabledShadow();
}

//================================CButtonUI============================

DRIECTUILIB_API CButtonUI* Delphi_ButtonUI_CppCreate() {
    return new CButtonUI();
}

DRIECTUILIB_API void Delphi_ButtonUI_CppDestroy(CButtonUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetClass(CButtonUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ButtonUI_GetInterface(CButtonUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_ButtonUI_GetControlFlags(CButtonUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API bool Delphi_ButtonUI_Activate(CButtonUI* handle) {
    return handle->Activate();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetEnabled(CButtonUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API void Delphi_ButtonUI_DoEvent(CButtonUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetNormalImage(CButtonUI* handle) {
    return handle->GetNormalImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetNormalImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetHotImage(CButtonUI* handle) {
    return handle->GetHotImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetHotImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetPushedImage(CButtonUI* handle) {
    return handle->GetPushedImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetPushedImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetPushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetFocusedImage(CButtonUI* handle) {
    return handle->GetFocusedImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetFocusedImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetFocusedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetDisabledImage(CButtonUI* handle) {
    return handle->GetDisabledImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetDisabledImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetDisabledImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetForeImage(CButtonUI* handle) {
    return handle->GetForeImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetForeImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetForeImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ButtonUI_GetHotForeImage(CButtonUI* handle) {
    return handle->GetHotForeImage();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetHotForeImage(CButtonUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotForeImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ButtonUI_SetHotBkColor(CButtonUI* handle ,DWORD dwColor) {
    handle->SetHotBkColor(dwColor);
}

DRIECTUILIB_API DWORD Delphi_ButtonUI_GetHotBkColor(CButtonUI* handle) {
    return handle->GetHotBkColor();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetHotTextColor(CButtonUI* handle ,DWORD dwColor) {
    handle->SetHotTextColor(dwColor);
}

DRIECTUILIB_API DWORD Delphi_ButtonUI_GetHotTextColor(CButtonUI* handle) {
    return handle->GetHotTextColor();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetPushedTextColor(CButtonUI* handle ,DWORD dwColor) {
    handle->SetPushedTextColor(dwColor);
}

DRIECTUILIB_API DWORD Delphi_ButtonUI_GetPushedTextColor(CButtonUI* handle) {
    return handle->GetPushedTextColor();
}

DRIECTUILIB_API void Delphi_ButtonUI_SetFocusedTextColor(CButtonUI* handle ,DWORD dwColor) {
    handle->SetFocusedTextColor(dwColor);
}

DRIECTUILIB_API DWORD Delphi_ButtonUI_GetFocusedTextColor(CButtonUI* handle) {
    return handle->GetFocusedTextColor();
}

DRIECTUILIB_API void Delphi_ButtonUI_EstimateSize(CButtonUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_ButtonUI_SetAttribute(CButtonUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ButtonUI_PaintText(CButtonUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

DRIECTUILIB_API void Delphi_ButtonUI_PaintStatusImage(CButtonUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

//================================COptionUI============================

DRIECTUILIB_API COptionUI* Delphi_OptionUI_CppCreate() {
    return new COptionUI();
}

DRIECTUILIB_API void Delphi_OptionUI_CppDestroy(COptionUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_OptionUI_GetClass(COptionUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_OptionUI_GetInterface(COptionUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_OptionUI_SetManager(COptionUI* handle ,CPaintManagerUI* pManager, CControlUI* pParent, bool bInit) {
    handle->SetManager(pManager, pParent, bInit);
}

DRIECTUILIB_API bool Delphi_OptionUI_Activate(COptionUI* handle) {
    return handle->Activate();
}

DRIECTUILIB_API void Delphi_OptionUI_SetEnabled(COptionUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API LPCTSTR Delphi_OptionUI_GetSelectedImage(COptionUI* handle) {
    return handle->GetSelectedImage();
}

DRIECTUILIB_API void Delphi_OptionUI_SetSelectedImage(COptionUI* handle ,LPCTSTR pStrImage) {
    handle->SetSelectedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_OptionUI_GetSelectedHotImage(COptionUI* handle) {
    return handle->GetSelectedHotImage();
}

DRIECTUILIB_API void Delphi_OptionUI_SetSelectedHotImage(COptionUI* handle ,LPCTSTR pStrImage) {
    handle->SetSelectedHotImage(pStrImage);
}

DRIECTUILIB_API void Delphi_OptionUI_SetSelectedTextColor(COptionUI* handle ,DWORD dwTextColor) {
    handle->SetSelectedTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_OptionUI_GetSelectedTextColor(COptionUI* handle) {
    return handle->GetSelectedTextColor();
}

DRIECTUILIB_API void Delphi_OptionUI_SetSelectedBkColor(COptionUI* handle ,DWORD dwBkColor) {
    handle->SetSelectedBkColor(dwBkColor);
}

DRIECTUILIB_API DWORD Delphi_OptionUI_GetSelectBkColor(COptionUI* handle) {
    return handle->GetSelectBkColor();
}

DRIECTUILIB_API LPCTSTR Delphi_OptionUI_GetForeImage(COptionUI* handle) {
    return handle->GetForeImage();
}

DRIECTUILIB_API void Delphi_OptionUI_SetForeImage(COptionUI* handle ,LPCTSTR pStrImage) {
    handle->SetForeImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_OptionUI_GetGroup(COptionUI* handle) {
    return handle->GetGroup();
}

DRIECTUILIB_API void Delphi_OptionUI_SetGroup(COptionUI* handle ,LPCTSTR pStrGroupName) {
    handle->SetGroup(pStrGroupName);
}

DRIECTUILIB_API bool Delphi_OptionUI_IsSelected(COptionUI* handle) {
    return handle->IsSelected();
}

DRIECTUILIB_API void Delphi_OptionUI_Selected(COptionUI* handle ,bool bSelected) {
    handle->Selected(bSelected);
}

DRIECTUILIB_API void Delphi_OptionUI_EstimateSize(COptionUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_OptionUI_SetAttribute(COptionUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_OptionUI_PaintStatusImage(COptionUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

DRIECTUILIB_API void Delphi_OptionUI_PaintText(COptionUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

//================================CCheckBoxUI============================

DRIECTUILIB_API CCheckBoxUI* Delphi_CheckBoxUI_CppCreate() {
    return new CCheckBoxUI();
}

DRIECTUILIB_API void Delphi_CheckBoxUI_CppDestroy(CCheckBoxUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_CheckBoxUI_GetClass(CCheckBoxUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_CheckBoxUI_GetInterface(CCheckBoxUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_CheckBoxUI_SetCheck(CCheckBoxUI* handle ,bool bCheck) {
    handle->SetCheck(bCheck);
}

DRIECTUILIB_API bool Delphi_CheckBoxUI_GetCheck(CCheckBoxUI* handle) {
    return handle->GetCheck();
}

//================================CListContainerElementUI============================

DRIECTUILIB_API CListContainerElementUI* Delphi_ListContainerElementUI_CppCreate() {
    return new CListContainerElementUI();
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_CppDestroy(CListContainerElementUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ListContainerElementUI_GetClass(CListContainerElementUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API UINT Delphi_ListContainerElementUI_GetControlFlags(CListContainerElementUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API LPVOID Delphi_ListContainerElementUI_GetInterface(CListContainerElementUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API int Delphi_ListContainerElementUI_GetIndex(CListContainerElementUI* handle) {
    return handle->GetIndex();
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_SetIndex(CListContainerElementUI* handle ,int iIndex) {
    handle->SetIndex(iIndex);
}

DRIECTUILIB_API IListOwnerUI* Delphi_ListContainerElementUI_GetOwner(CListContainerElementUI* handle) {
    return handle->GetOwner();
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_SetOwner(CListContainerElementUI* handle ,CControlUI* pOwner) {
    handle->SetOwner(pOwner);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_SetVisible(CListContainerElementUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_SetEnabled(CListContainerElementUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API bool Delphi_ListContainerElementUI_IsSelected(CListContainerElementUI* handle) {
    return handle->IsSelected();
}

DRIECTUILIB_API bool Delphi_ListContainerElementUI_Select(CListContainerElementUI* handle ,bool bSelect) {
    return handle->Select(bSelect);
}

DRIECTUILIB_API bool Delphi_ListContainerElementUI_IsExpanded(CListContainerElementUI* handle) {
    return handle->IsExpanded();
}

DRIECTUILIB_API bool Delphi_ListContainerElementUI_Expand(CListContainerElementUI* handle ,bool bExpand) {
    return handle->Expand(bExpand);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_Invalidate(CListContainerElementUI* handle) {
    handle->Invalidate();
}

DRIECTUILIB_API bool Delphi_ListContainerElementUI_Activate(CListContainerElementUI* handle) {
    return handle->Activate();
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_DoEvent(CListContainerElementUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_SetAttribute(CListContainerElementUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_DoPaint(CListContainerElementUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_DrawItemText(CListContainerElementUI* handle ,HDC hDC, RECT& rcItem) {
    handle->DrawItemText(hDC, rcItem);
}

DRIECTUILIB_API void Delphi_ListContainerElementUI_DrawItemBk(CListContainerElementUI* handle ,HDC hDC, RECT& rcItem) {
    handle->DrawItemBk(hDC, rcItem);
}

//================================CComboUI============================

DRIECTUILIB_API CComboUI* Delphi_ComboUI_CppCreate() {
    return new CComboUI();
}

DRIECTUILIB_API void Delphi_ComboUI_CppDestroy(CComboUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetClass(CComboUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ComboUI_GetInterface(CComboUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_ComboUI_DoInit(CComboUI* handle) {
    handle->DoInit();
}

DRIECTUILIB_API UINT Delphi_ComboUI_GetControlFlags(CComboUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API CDuiString Delphi_ComboUI_GetText(CComboUI* handle) {
    return handle->GetText();
}

DRIECTUILIB_API void Delphi_ComboUI_SetEnabled(CComboUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API CDuiString Delphi_ComboUI_GetDropBoxAttributeList(CComboUI* handle) {
    return handle->GetDropBoxAttributeList();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDropBoxAttributeList(CComboUI* handle ,LPCTSTR pstrList) {
    handle->SetDropBoxAttributeList(pstrList);
}

DRIECTUILIB_API void Delphi_ComboUI_GetDropBoxSize(CComboUI* handle, SIZE& Result) {
    Result = handle->GetDropBoxSize();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDropBoxSize(CComboUI* handle ,SIZE szDropBox) {
    handle->SetDropBoxSize(szDropBox);
}

DRIECTUILIB_API int Delphi_ComboUI_GetCurSel(CComboUI* handle) {
    return handle->GetCurSel();
}

DRIECTUILIB_API bool Delphi_ComboUI_GetSelectCloseFlag(CComboUI* handle) {
    return handle->GetSelectCloseFlag();
}

DRIECTUILIB_API void Delphi_ComboUI_SetSelectCloseFlag(CComboUI* handle ,bool flag) {
    handle->SetSelectCloseFlag(flag);
}

DRIECTUILIB_API bool Delphi_ComboUI_SelectItem(CComboUI* handle ,int iIndex, bool bTakeFocus) {
    return handle->SelectItem(iIndex, bTakeFocus);
}

DRIECTUILIB_API bool Delphi_ComboUI_SetItemIndex(CComboUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->SetItemIndex(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_ComboUI_Add(CComboUI* handle ,CControlUI* pControl) {
    return handle->Add(pControl);
}

DRIECTUILIB_API bool Delphi_ComboUI_AddAt(CComboUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_ComboUI_Remove(CComboUI* handle ,CControlUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API bool Delphi_ComboUI_RemoveAt(CComboUI* handle ,int iIndex) {
    return handle->RemoveAt(iIndex);
}

DRIECTUILIB_API void Delphi_ComboUI_RemoveAll(CComboUI* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API bool Delphi_ComboUI_Activate(CComboUI* handle) {
    return handle->Activate();
}

DRIECTUILIB_API bool Delphi_ComboUI_GetShowText(CComboUI* handle) {
    return handle->GetShowText();
}

DRIECTUILIB_API void Delphi_ComboUI_SetShowText(CComboUI* handle ,bool flag) {
    handle->SetShowText(flag);
}

DRIECTUILIB_API void Delphi_ComboUI_GetTextPadding(CComboUI* handle, RECT& Result) {
    Result = handle->GetTextPadding();
}

DRIECTUILIB_API void Delphi_ComboUI_SetTextPadding(CComboUI* handle ,RECT rc) {
    handle->SetTextPadding(rc);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetNormalImage(CComboUI* handle) {
    return handle->GetNormalImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetNormalImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetHotImage(CComboUI* handle) {
    return handle->GetHotImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetHotImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetPushedImage(CComboUI* handle) {
    return handle->GetPushedImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetPushedImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetPushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetFocusedImage(CComboUI* handle) {
    return handle->GetFocusedImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetFocusedImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetFocusedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetDisabledImage(CComboUI* handle) {
    return handle->GetDisabledImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDisabledImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetDisabledImage(pStrImage);
}

DRIECTUILIB_API TListInfoUI* Delphi_ComboUI_GetListInfo(CComboUI* handle) {
    return handle->GetListInfo();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemFont(CComboUI* handle ,int index) {
    handle->SetItemFont(index);
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemTextStyle(CComboUI* handle ,UINT uStyle) {
    handle->SetItemTextStyle(uStyle);
}

DRIECTUILIB_API void Delphi_ComboUI_GetItemTextPadding(CComboUI* handle, RECT& Result) {
    Result = handle->GetItemTextPadding();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemTextPadding(CComboUI* handle ,RECT rc) {
    handle->SetItemTextPadding(rc);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetItemTextColor(CComboUI* handle) {
    return handle->GetItemTextColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemTextColor(CComboUI* handle ,DWORD dwTextColor) {
    handle->SetItemTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetItemBkColor(CComboUI* handle) {
    return handle->GetItemBkColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemBkColor(CComboUI* handle ,DWORD dwBkColor) {
    handle->SetItemBkColor(dwBkColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetItemBkImage(CComboUI* handle) {
    return handle->GetItemBkImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemBkImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetItemBkImage(pStrImage);
}

DRIECTUILIB_API bool Delphi_ComboUI_IsAlternateBk(CComboUI* handle) {
    return handle->IsAlternateBk();
}

DRIECTUILIB_API void Delphi_ComboUI_SetAlternateBk(CComboUI* handle ,bool bAlternateBk) {
    handle->SetAlternateBk(bAlternateBk);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetSelectedItemTextColor(CComboUI* handle) {
    return handle->GetSelectedItemTextColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetSelectedItemTextColor(CComboUI* handle ,DWORD dwTextColor) {
    handle->SetSelectedItemTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetSelectedItemBkColor(CComboUI* handle) {
    return handle->GetSelectedItemBkColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetSelectedItemBkColor(CComboUI* handle ,DWORD dwBkColor) {
    handle->SetSelectedItemBkColor(dwBkColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetSelectedItemImage(CComboUI* handle) {
    return handle->GetSelectedItemImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetSelectedItemImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetSelectedItemImage(pStrImage);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetHotItemTextColor(CComboUI* handle) {
    return handle->GetHotItemTextColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetHotItemTextColor(CComboUI* handle ,DWORD dwTextColor) {
    handle->SetHotItemTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetHotItemBkColor(CComboUI* handle) {
    return handle->GetHotItemBkColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetHotItemBkColor(CComboUI* handle ,DWORD dwBkColor) {
    handle->SetHotItemBkColor(dwBkColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetHotItemImage(CComboUI* handle) {
    return handle->GetHotItemImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetHotItemImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotItemImage(pStrImage);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetDisabledItemTextColor(CComboUI* handle) {
    return handle->GetDisabledItemTextColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDisabledItemTextColor(CComboUI* handle ,DWORD dwTextColor) {
    handle->SetDisabledItemTextColor(dwTextColor);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetDisabledItemBkColor(CComboUI* handle) {
    return handle->GetDisabledItemBkColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDisabledItemBkColor(CComboUI* handle ,DWORD dwBkColor) {
    handle->SetDisabledItemBkColor(dwBkColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ComboUI_GetDisabledItemImage(CComboUI* handle) {
    return handle->GetDisabledItemImage();
}

DRIECTUILIB_API void Delphi_ComboUI_SetDisabledItemImage(CComboUI* handle ,LPCTSTR pStrImage) {
    handle->SetDisabledItemImage(pStrImage);
}

DRIECTUILIB_API DWORD Delphi_ComboUI_GetItemLineColor(CComboUI* handle) {
    return handle->GetItemLineColor();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemLineColor(CComboUI* handle ,DWORD dwLineColor) {
    handle->SetItemLineColor(dwLineColor);
}

DRIECTUILIB_API bool Delphi_ComboUI_IsItemShowHtml(CComboUI* handle) {
    return handle->IsItemShowHtml();
}

DRIECTUILIB_API void Delphi_ComboUI_SetItemShowHtml(CComboUI* handle ,bool bShowHtml) {
    handle->SetItemShowHtml(bShowHtml);
}

DRIECTUILIB_API void Delphi_ComboUI_EstimateSize(CComboUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_ComboUI_SetPos(CComboUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ComboUI_Move(CComboUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ComboUI_DoEvent(CComboUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ComboUI_SetAttribute(CComboUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ComboUI_DoPaint(CComboUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ComboUI_PaintText(CComboUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

DRIECTUILIB_API void Delphi_ComboUI_PaintStatusImage(CComboUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

//================================CDateTimeUI============================

DRIECTUILIB_API CDateTimeUI* Delphi_DateTimeUI_CppCreate() {
    return new CDateTimeUI();
}

DRIECTUILIB_API void Delphi_DateTimeUI_CppDestroy(CDateTimeUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_DateTimeUI_GetClass(CDateTimeUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_DateTimeUI_GetInterface(CDateTimeUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API SYSTEMTIME& Delphi_DateTimeUI_GetTime(CDateTimeUI* handle) {
    return handle->GetTime();
}

DRIECTUILIB_API void Delphi_DateTimeUI_SetTime(CDateTimeUI* handle ,SYSTEMTIME* pst) {
    handle->SetTime(pst);
}

DRIECTUILIB_API void Delphi_DateTimeUI_SetReadOnly(CDateTimeUI* handle ,bool bReadOnly) {
    handle->SetReadOnly(bReadOnly);
}

DRIECTUILIB_API bool Delphi_DateTimeUI_IsReadOnly(CDateTimeUI* handle) {
    return handle->IsReadOnly();
}

DRIECTUILIB_API void Delphi_DateTimeUI_UpdateText(CDateTimeUI* handle) {
    handle->UpdateText();
}

DRIECTUILIB_API void Delphi_DateTimeUI_DoEvent(CDateTimeUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

//================================CEditUI============================

DRIECTUILIB_API CEditUI* Delphi_EditUI_CppCreate() {
    return new CEditUI();
}

DRIECTUILIB_API void Delphi_EditUI_CppDestroy(CEditUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_EditUI_GetClass(CEditUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_EditUI_GetInterface(CEditUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_EditUI_GetControlFlags(CEditUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API void Delphi_EditUI_SetEnabled(CEditUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API void Delphi_EditUI_SetText(CEditUI* handle ,LPCTSTR pstrText) {
    handle->SetText(pstrText);
}

DRIECTUILIB_API void Delphi_EditUI_SetMaxChar(CEditUI* handle ,UINT uMax) {
    handle->SetMaxChar(uMax);
}

DRIECTUILIB_API UINT Delphi_EditUI_GetMaxChar(CEditUI* handle) {
    return handle->GetMaxChar();
}

DRIECTUILIB_API void Delphi_EditUI_SetReadOnly(CEditUI* handle ,bool bReadOnly) {
    handle->SetReadOnly(bReadOnly);
}

DRIECTUILIB_API bool Delphi_EditUI_IsReadOnly(CEditUI* handle) {
    return handle->IsReadOnly();
}

DRIECTUILIB_API void Delphi_EditUI_SetPasswordMode(CEditUI* handle ,bool bPasswordMode) {
    handle->SetPasswordMode(bPasswordMode);
}

DRIECTUILIB_API bool Delphi_EditUI_IsPasswordMode(CEditUI* handle) {
    return handle->IsPasswordMode();
}

DRIECTUILIB_API void Delphi_EditUI_SetPasswordChar(CEditUI* handle ,TCHAR cPasswordChar) {
    handle->SetPasswordChar(cPasswordChar);
}

DRIECTUILIB_API TCHAR Delphi_EditUI_GetPasswordChar(CEditUI* handle) {
    return handle->GetPasswordChar();
}

DRIECTUILIB_API void Delphi_EditUI_SetNumberOnly(CEditUI* handle ,bool bNumberOnly) {
    handle->SetNumberOnly(bNumberOnly);
}

DRIECTUILIB_API bool Delphi_EditUI_IsNumberOnly(CEditUI* handle) {
    return handle->IsNumberOnly();
}

DRIECTUILIB_API int Delphi_EditUI_GetWindowStyls(CEditUI* handle) {
    return handle->GetWindowStyls();
}

DRIECTUILIB_API HWND Delphi_EditUI_GetNativeEditHWND(CEditUI* handle) {
    return handle->GetNativeEditHWND();
}

DRIECTUILIB_API LPCTSTR Delphi_EditUI_GetNormalImage(CEditUI* handle) {
    return handle->GetNormalImage();
}

DRIECTUILIB_API void Delphi_EditUI_SetNormalImage(CEditUI* handle ,LPCTSTR pStrImage) {
    handle->SetNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_EditUI_GetHotImage(CEditUI* handle) {
    return handle->GetHotImage();
}

DRIECTUILIB_API void Delphi_EditUI_SetHotImage(CEditUI* handle ,LPCTSTR pStrImage) {
    handle->SetHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_EditUI_GetFocusedImage(CEditUI* handle) {
    return handle->GetFocusedImage();
}

DRIECTUILIB_API void Delphi_EditUI_SetFocusedImage(CEditUI* handle ,LPCTSTR pStrImage) {
    handle->SetFocusedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_EditUI_GetDisabledImage(CEditUI* handle) {
    return handle->GetDisabledImage();
}

DRIECTUILIB_API void Delphi_EditUI_SetDisabledImage(CEditUI* handle ,LPCTSTR pStrImage) {
    handle->SetDisabledImage(pStrImage);
}

DRIECTUILIB_API void Delphi_EditUI_SetNativeEditBkColor(CEditUI* handle ,DWORD dwBkColor) {
    handle->SetNativeEditBkColor(dwBkColor);
}

DRIECTUILIB_API DWORD Delphi_EditUI_GetNativeEditBkColor(CEditUI* handle) {
    return handle->GetNativeEditBkColor();
}

DRIECTUILIB_API void Delphi_EditUI_SetSel(CEditUI* handle ,long nStartChar, long nEndChar) {
    handle->SetSel(nStartChar, nEndChar);
}

DRIECTUILIB_API void Delphi_EditUI_SetSelAll(CEditUI* handle) {
    handle->SetSelAll();
}

DRIECTUILIB_API void Delphi_EditUI_SetReplaceSel(CEditUI* handle ,LPCTSTR lpszReplace) {
    handle->SetReplaceSel(lpszReplace);
}

DRIECTUILIB_API void Delphi_EditUI_SetPos(CEditUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_EditUI_Move(CEditUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_EditUI_SetVisible(CEditUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_EditUI_SetInternVisible(CEditUI* handle ,bool bVisible) {
    handle->SetInternVisible(bVisible);
}

DRIECTUILIB_API void Delphi_EditUI_EstimateSize(CEditUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_EditUI_DoEvent(CEditUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_EditUI_SetAttribute(CEditUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_EditUI_PaintStatusImage(CEditUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

DRIECTUILIB_API void Delphi_EditUI_PaintText(CEditUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

//================================CProgressUI============================

DRIECTUILIB_API CProgressUI* Delphi_ProgressUI_CppCreate() {
    return new CProgressUI();
}

DRIECTUILIB_API void Delphi_ProgressUI_CppDestroy(CProgressUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ProgressUI_GetClass(CProgressUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ProgressUI_GetInterface(CProgressUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API bool Delphi_ProgressUI_IsHorizontal(CProgressUI* handle) {
    return handle->IsHorizontal();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetHorizontal(CProgressUI* handle ,bool bHorizontal) {
    handle->SetHorizontal(bHorizontal);
}

DRIECTUILIB_API bool Delphi_ProgressUI_IsStretchForeImage(CProgressUI* handle) {
    return handle->IsStretchForeImage();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetStretchForeImage(CProgressUI* handle ,bool bStretchForeImage) {
    handle->SetStretchForeImage(bStretchForeImage);
}

DRIECTUILIB_API int Delphi_ProgressUI_GetMinValue(CProgressUI* handle) {
    return handle->GetMinValue();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetMinValue(CProgressUI* handle ,int nMin) {
    handle->SetMinValue(nMin);
}

DRIECTUILIB_API int Delphi_ProgressUI_GetMaxValue(CProgressUI* handle) {
    return handle->GetMaxValue();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetMaxValue(CProgressUI* handle ,int nMax) {
    handle->SetMaxValue(nMax);
}

DRIECTUILIB_API int Delphi_ProgressUI_GetValue(CProgressUI* handle) {
    return handle->GetValue();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetValue(CProgressUI* handle ,int nValue) {
    handle->SetValue(nValue);
}

DRIECTUILIB_API LPCTSTR Delphi_ProgressUI_GetForeImage(CProgressUI* handle) {
    return handle->GetForeImage();
}

DRIECTUILIB_API void Delphi_ProgressUI_SetForeImage(CProgressUI* handle ,LPCTSTR pStrImage) {
    handle->SetForeImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ProgressUI_SetAttribute(CProgressUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ProgressUI_PaintStatusImage(CProgressUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

//================================CRichEditUI============================

DRIECTUILIB_API CRichEditUI* Delphi_RichEditUI_CppCreate() {
    return new CRichEditUI();
}

DRIECTUILIB_API void Delphi_RichEditUI_CppDestroy(CRichEditUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_RichEditUI_GetClass(CRichEditUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_RichEditUI_GetInterface(CRichEditUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_RichEditUI_GetControlFlags(CRichEditUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API bool Delphi_RichEditUI_IsWantTab(CRichEditUI* handle) {
    return handle->IsWantTab();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetWantTab(CRichEditUI* handle ,bool bWantTab) {
    handle->SetWantTab(bWantTab);
}

DRIECTUILIB_API bool Delphi_RichEditUI_IsWantReturn(CRichEditUI* handle) {
    return handle->IsWantReturn();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetWantReturn(CRichEditUI* handle ,bool bWantReturn) {
    handle->SetWantReturn(bWantReturn);
}

DRIECTUILIB_API bool Delphi_RichEditUI_IsWantCtrlReturn(CRichEditUI* handle) {
    return handle->IsWantCtrlReturn();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetWantCtrlReturn(CRichEditUI* handle ,bool bWantCtrlReturn) {
    handle->SetWantCtrlReturn(bWantCtrlReturn);
}

DRIECTUILIB_API bool Delphi_RichEditUI_IsRich(CRichEditUI* handle) {
    return handle->IsRich();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetRich(CRichEditUI* handle ,bool bRich) {
    handle->SetRich(bRich);
}

DRIECTUILIB_API bool Delphi_RichEditUI_IsReadOnly(CRichEditUI* handle) {
    return handle->IsReadOnly();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetReadOnly(CRichEditUI* handle ,bool bReadOnly) {
    handle->SetReadOnly(bReadOnly);
}

DRIECTUILIB_API bool Delphi_RichEditUI_GetWordWrap(CRichEditUI* handle) {
    return handle->GetWordWrap();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetWordWrap(CRichEditUI* handle ,bool bWordWrap) {
    handle->SetWordWrap(bWordWrap);
}

DRIECTUILIB_API int Delphi_RichEditUI_GetFont(CRichEditUI* handle) {
    return handle->GetFont();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetFont_01(CRichEditUI* handle ,int index) {
    handle->SetFont(index);
}

DRIECTUILIB_API void Delphi_RichEditUI_SetFont_02(CRichEditUI* handle ,LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic) {
    handle->SetFont(pStrFontName, nSize, bBold, bUnderline, bItalic);
}

DRIECTUILIB_API LONG Delphi_RichEditUI_GetWinStyle(CRichEditUI* handle) {
    return handle->GetWinStyle();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetWinStyle(CRichEditUI* handle ,LONG lStyle) {
    handle->SetWinStyle(lStyle);
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_GetTextColor(CRichEditUI* handle) {
    return handle->GetTextColor();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetTextColor(CRichEditUI* handle ,DWORD dwTextColor) {
    handle->SetTextColor(dwTextColor);
}

DRIECTUILIB_API int Delphi_RichEditUI_GetLimitText(CRichEditUI* handle) {
    return handle->GetLimitText();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetLimitText(CRichEditUI* handle ,int iChars) {
    handle->SetLimitText(iChars);
}

DRIECTUILIB_API long Delphi_RichEditUI_GetTextLength(CRichEditUI* handle ,DWORD dwFlags) {
    return handle->GetTextLength(dwFlags);
}

DRIECTUILIB_API CDuiString Delphi_RichEditUI_GetText(CRichEditUI* handle) {
    return handle->GetText();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetText(CRichEditUI* handle ,LPCTSTR pstrText) {
    handle->SetText(pstrText);
}

DRIECTUILIB_API bool Delphi_RichEditUI_GetModify(CRichEditUI* handle) {
    return handle->GetModify();
}

DRIECTUILIB_API void Delphi_RichEditUI_SetModify(CRichEditUI* handle ,bool bModified) {
    handle->SetModify(bModified);
}

DRIECTUILIB_API void Delphi_RichEditUI_GetSel_01(CRichEditUI* handle ,CHARRANGE& cr) {
    handle->GetSel(cr);
}

DRIECTUILIB_API void Delphi_RichEditUI_GetSel_02(CRichEditUI* handle ,long& nStartChar, long& nEndChar) {
    handle->GetSel(nStartChar, nEndChar);
}

DRIECTUILIB_API int Delphi_RichEditUI_SetSel_01(CRichEditUI* handle ,CHARRANGE& cr) {
    return handle->SetSel(cr);
}

DRIECTUILIB_API int Delphi_RichEditUI_SetSel_02(CRichEditUI* handle ,long nStartChar, long nEndChar) {
    return handle->SetSel(nStartChar, nEndChar);
}

DRIECTUILIB_API void Delphi_RichEditUI_ReplaceSel(CRichEditUI* handle ,LPCTSTR lpszNewText, bool bCanUndo) {
    handle->ReplaceSel(lpszNewText, bCanUndo);
}

DRIECTUILIB_API void Delphi_RichEditUI_ReplaceSelW(CRichEditUI* handle ,LPCWSTR lpszNewText, bool bCanUndo) {
    handle->ReplaceSelW(lpszNewText, bCanUndo);
}

DRIECTUILIB_API CDuiString Delphi_RichEditUI_GetSelText(CRichEditUI* handle) {
    return handle->GetSelText();
}

DRIECTUILIB_API int Delphi_RichEditUI_SetSelAll(CRichEditUI* handle) {
    return handle->SetSelAll();
}

DRIECTUILIB_API int Delphi_RichEditUI_SetSelNone(CRichEditUI* handle) {
    return handle->SetSelNone();
}

DRIECTUILIB_API WORD Delphi_RichEditUI_GetSelectionType(CRichEditUI* handle) {
    return handle->GetSelectionType();
}

DRIECTUILIB_API bool Delphi_RichEditUI_GetZoom(CRichEditUI* handle ,int& nNum, int& nDen) {
    return handle->GetZoom(nNum, nDen);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetZoom(CRichEditUI* handle ,int nNum, int nDen) {
    return handle->SetZoom(nNum, nDen);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetZoomOff(CRichEditUI* handle) {
    return handle->SetZoomOff();
}

DRIECTUILIB_API bool Delphi_RichEditUI_GetAutoURLDetect(CRichEditUI* handle) {
    return handle->GetAutoURLDetect();
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetAutoURLDetect(CRichEditUI* handle ,bool bAutoDetect) {
    return handle->SetAutoURLDetect(bAutoDetect);
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_GetEventMask(CRichEditUI* handle) {
    return handle->GetEventMask();
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_SetEventMask(CRichEditUI* handle ,DWORD dwEventMask) {
    return handle->SetEventMask(dwEventMask);
}

DRIECTUILIB_API CDuiString Delphi_RichEditUI_GetTextRange(CRichEditUI* handle ,long nStartChar, long nEndChar) {
    return handle->GetTextRange(nStartChar, nEndChar);
}

DRIECTUILIB_API void Delphi_RichEditUI_HideSelection(CRichEditUI* handle ,bool bHide, bool bChangeStyle) {
    handle->HideSelection(bHide, bChangeStyle);
}

DRIECTUILIB_API void Delphi_RichEditUI_ScrollCaret(CRichEditUI* handle) {
    handle->ScrollCaret();
}

DRIECTUILIB_API int Delphi_RichEditUI_InsertText(CRichEditUI* handle ,long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo) {
    return handle->InsertText(nInsertAfterChar, lpstrText, bCanUndo);
}

DRIECTUILIB_API int Delphi_RichEditUI_AppendText(CRichEditUI* handle ,LPCTSTR lpstrText, bool bCanUndo) {
    return handle->AppendText(lpstrText, bCanUndo);
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_GetDefaultCharFormat(CRichEditUI* handle ,CHARFORMAT2& cf) {
    return handle->GetDefaultCharFormat(cf);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetDefaultCharFormat(CRichEditUI* handle ,CHARFORMAT2& cf) {
    return handle->SetDefaultCharFormat(cf);
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_GetSelectionCharFormat(CRichEditUI* handle ,CHARFORMAT2& cf) {
    return handle->GetSelectionCharFormat(cf);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetSelectionCharFormat(CRichEditUI* handle ,CHARFORMAT2& cf) {
    return handle->SetSelectionCharFormat(cf);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetWordCharFormat(CRichEditUI* handle ,CHARFORMAT2& cf) {
    return handle->SetWordCharFormat(cf);
}

DRIECTUILIB_API DWORD Delphi_RichEditUI_GetParaFormat(CRichEditUI* handle ,PARAFORMAT2& pf) {
    return handle->GetParaFormat(pf);
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetParaFormat(CRichEditUI* handle ,PARAFORMAT2& pf) {
    return handle->SetParaFormat(pf);
}

DRIECTUILIB_API bool Delphi_RichEditUI_Redo(CRichEditUI* handle) {
    return handle->Redo();
}

DRIECTUILIB_API bool Delphi_RichEditUI_Undo(CRichEditUI* handle) {
    return handle->Undo();
}

DRIECTUILIB_API void Delphi_RichEditUI_Clear(CRichEditUI* handle) {
    handle->Clear();
}

DRIECTUILIB_API void Delphi_RichEditUI_Copy(CRichEditUI* handle) {
    handle->Copy();
}

DRIECTUILIB_API void Delphi_RichEditUI_Cut(CRichEditUI* handle) {
    handle->Cut();
}

DRIECTUILIB_API void Delphi_RichEditUI_Paste(CRichEditUI* handle) {
    handle->Paste();
}

DRIECTUILIB_API int Delphi_RichEditUI_GetLineCount(CRichEditUI* handle) {
    return handle->GetLineCount();
}

DRIECTUILIB_API CDuiString Delphi_RichEditUI_GetLine(CRichEditUI* handle ,int nIndex, int nMaxLength) {
    return handle->GetLine(nIndex, nMaxLength);
}

DRIECTUILIB_API int Delphi_RichEditUI_LineIndex(CRichEditUI* handle ,int nLine) {
    return handle->LineIndex(nLine);
}

DRIECTUILIB_API int Delphi_RichEditUI_LineLength(CRichEditUI* handle ,int nLine) {
    return handle->LineLength(nLine);
}

DRIECTUILIB_API bool Delphi_RichEditUI_LineScroll(CRichEditUI* handle ,int nLines, int nChars) {
    return handle->LineScroll(nLines, nChars);
}

DRIECTUILIB_API CDuiPoint Delphi_RichEditUI_GetCharPos(CRichEditUI* handle ,long lChar) {
    return handle->GetCharPos(lChar);
}

DRIECTUILIB_API long Delphi_RichEditUI_LineFromChar(CRichEditUI* handle ,long nIndex) {
    return handle->LineFromChar(nIndex);
}

DRIECTUILIB_API CDuiPoint Delphi_RichEditUI_PosFromChar(CRichEditUI* handle ,UINT nChar) {
    return handle->PosFromChar(nChar);
}

DRIECTUILIB_API int Delphi_RichEditUI_CharFromPos(CRichEditUI* handle ,CDuiPoint pt) {
    return handle->CharFromPos(pt);
}

DRIECTUILIB_API void Delphi_RichEditUI_EmptyUndoBuffer(CRichEditUI* handle) {
    handle->EmptyUndoBuffer();
}

DRIECTUILIB_API UINT Delphi_RichEditUI_SetUndoLimit(CRichEditUI* handle ,UINT nLimit) {
    return handle->SetUndoLimit(nLimit);
}

DRIECTUILIB_API long Delphi_RichEditUI_StreamIn(CRichEditUI* handle ,int nFormat, EDITSTREAM& es) {
    return handle->StreamIn(nFormat, es);
}

DRIECTUILIB_API long Delphi_RichEditUI_StreamOut(CRichEditUI* handle ,int nFormat, EDITSTREAM& es) {
    return handle->StreamOut(nFormat, es);
}

DRIECTUILIB_API void Delphi_RichEditUI_DoInit(CRichEditUI* handle) {
    handle->DoInit();
}

DRIECTUILIB_API bool Delphi_RichEditUI_SetDropAcceptFile(CRichEditUI* handle ,bool bAccept) {
    return handle->SetDropAcceptFile(bAccept);
}

DRIECTUILIB_API HRESULT Delphi_RichEditUI_TxSendMessage(CRichEditUI* handle ,UINT msg, WPARAM wparam, LPARAM lparam, LRESULT* plresult) {
    return handle->TxSendMessage(msg, wparam, lparam, plresult);
}

DRIECTUILIB_API IDropTarget* Delphi_RichEditUI_GetTxDropTarget(CRichEditUI* handle) {
    return handle->GetTxDropTarget();
}

DRIECTUILIB_API bool Delphi_RichEditUI_OnTxViewChanged(CRichEditUI* handle) {
    return handle->OnTxViewChanged();
}

DRIECTUILIB_API void Delphi_RichEditUI_OnTxNotify(CRichEditUI* handle ,DWORD iNotify, void* pv) {
    handle->OnTxNotify(iNotify, pv);
}

DRIECTUILIB_API void Delphi_RichEditUI_SetScrollPos(CRichEditUI* handle ,SIZE szPos) {
    handle->SetScrollPos(szPos);
}

DRIECTUILIB_API void Delphi_RichEditUI_LineUp(CRichEditUI* handle) {
    handle->LineUp();
}

DRIECTUILIB_API void Delphi_RichEditUI_LineDown(CRichEditUI* handle) {
    handle->LineDown();
}

DRIECTUILIB_API void Delphi_RichEditUI_PageUp(CRichEditUI* handle) {
    handle->PageUp();
}

DRIECTUILIB_API void Delphi_RichEditUI_PageDown(CRichEditUI* handle) {
    handle->PageDown();
}

DRIECTUILIB_API void Delphi_RichEditUI_HomeUp(CRichEditUI* handle) {
    handle->HomeUp();
}

DRIECTUILIB_API void Delphi_RichEditUI_EndDown(CRichEditUI* handle) {
    handle->EndDown();
}

DRIECTUILIB_API void Delphi_RichEditUI_LineLeft(CRichEditUI* handle) {
    handle->LineLeft();
}

DRIECTUILIB_API void Delphi_RichEditUI_LineRight(CRichEditUI* handle) {
    handle->LineRight();
}

DRIECTUILIB_API void Delphi_RichEditUI_PageLeft(CRichEditUI* handle) {
    handle->PageLeft();
}

DRIECTUILIB_API void Delphi_RichEditUI_PageRight(CRichEditUI* handle) {
    handle->PageRight();
}

DRIECTUILIB_API void Delphi_RichEditUI_HomeLeft(CRichEditUI* handle) {
    handle->HomeLeft();
}

DRIECTUILIB_API void Delphi_RichEditUI_EndRight(CRichEditUI* handle) {
    handle->EndRight();
}

DRIECTUILIB_API void Delphi_RichEditUI_EstimateSize(CRichEditUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_RichEditUI_SetPos(CRichEditUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_RichEditUI_Move(CRichEditUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_RichEditUI_DoEvent(CRichEditUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_RichEditUI_DoPaint(CRichEditUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_RichEditUI_SetAttribute(CRichEditUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

//================================CScrollBarUI============================

DRIECTUILIB_API CScrollBarUI* Delphi_ScrollBarUI_CppCreate() {
    return new CScrollBarUI();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_CppDestroy(CScrollBarUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetClass(CScrollBarUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ScrollBarUI_GetInterface(CScrollBarUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API CContainerUI* Delphi_ScrollBarUI_GetOwner(CScrollBarUI* handle) {
    return handle->GetOwner();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetOwner(CScrollBarUI* handle ,CContainerUI* pOwner) {
    handle->SetOwner(pOwner);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetVisible(CScrollBarUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetEnabled(CScrollBarUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetFocus(CScrollBarUI* handle) {
    handle->SetFocus();
}

DRIECTUILIB_API bool Delphi_ScrollBarUI_IsHorizontal(CScrollBarUI* handle) {
    return handle->IsHorizontal();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetHorizontal(CScrollBarUI* handle ,bool bHorizontal) {
    handle->SetHorizontal(bHorizontal);
}

DRIECTUILIB_API int Delphi_ScrollBarUI_GetScrollRange(CScrollBarUI* handle) {
    return handle->GetScrollRange();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetScrollRange(CScrollBarUI* handle ,int nRange) {
    handle->SetScrollRange(nRange);
}

DRIECTUILIB_API int Delphi_ScrollBarUI_GetScrollPos(CScrollBarUI* handle) {
    return handle->GetScrollPos();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetScrollPos(CScrollBarUI* handle ,int nPos) {
    handle->SetScrollPos(nPos);
}

DRIECTUILIB_API int Delphi_ScrollBarUI_GetLineSize(CScrollBarUI* handle) {
    return handle->GetLineSize();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetLineSize(CScrollBarUI* handle ,int nSize) {
    handle->SetLineSize(nSize);
}

DRIECTUILIB_API bool Delphi_ScrollBarUI_GetShowButton1(CScrollBarUI* handle) {
    return handle->GetShowButton1();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetShowButton1(CScrollBarUI* handle ,bool bShow) {
    handle->SetShowButton1(bShow);
}

DRIECTUILIB_API DWORD Delphi_ScrollBarUI_GetButton1Color(CScrollBarUI* handle) {
    return handle->GetButton1Color();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton1Color(CScrollBarUI* handle ,DWORD dwColor) {
    handle->SetButton1Color(dwColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton1NormalImage(CScrollBarUI* handle) {
    return handle->GetButton1NormalImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton1NormalImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton1NormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton1HotImage(CScrollBarUI* handle) {
    return handle->GetButton1HotImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton1HotImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton1HotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton1PushedImage(CScrollBarUI* handle) {
    return handle->GetButton1PushedImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton1PushedImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton1PushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton1DisabledImage(CScrollBarUI* handle) {
    return handle->GetButton1DisabledImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton1DisabledImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton1DisabledImage(pStrImage);
}

DRIECTUILIB_API bool Delphi_ScrollBarUI_GetShowButton2(CScrollBarUI* handle) {
    return handle->GetShowButton2();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetShowButton2(CScrollBarUI* handle ,bool bShow) {
    handle->SetShowButton2(bShow);
}

DRIECTUILIB_API DWORD Delphi_ScrollBarUI_GetButton2Color(CScrollBarUI* handle) {
    return handle->GetButton2Color();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton2Color(CScrollBarUI* handle ,DWORD dwColor) {
    handle->SetButton2Color(dwColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton2NormalImage(CScrollBarUI* handle) {
    return handle->GetButton2NormalImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton2NormalImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton2NormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton2HotImage(CScrollBarUI* handle) {
    return handle->GetButton2HotImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton2HotImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton2HotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton2PushedImage(CScrollBarUI* handle) {
    return handle->GetButton2PushedImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton2PushedImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton2PushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetButton2DisabledImage(CScrollBarUI* handle) {
    return handle->GetButton2DisabledImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetButton2DisabledImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetButton2DisabledImage(pStrImage);
}

DRIECTUILIB_API DWORD Delphi_ScrollBarUI_GetThumbColor(CScrollBarUI* handle) {
    return handle->GetThumbColor();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetThumbColor(CScrollBarUI* handle ,DWORD dwColor) {
    handle->SetThumbColor(dwColor);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetThumbNormalImage(CScrollBarUI* handle) {
    return handle->GetThumbNormalImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetThumbNormalImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetThumbHotImage(CScrollBarUI* handle) {
    return handle->GetThumbHotImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetThumbHotImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetThumbPushedImage(CScrollBarUI* handle) {
    return handle->GetThumbPushedImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetThumbPushedImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbPushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetThumbDisabledImage(CScrollBarUI* handle) {
    return handle->GetThumbDisabledImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetThumbDisabledImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbDisabledImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetRailNormalImage(CScrollBarUI* handle) {
    return handle->GetRailNormalImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetRailNormalImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetRailNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetRailHotImage(CScrollBarUI* handle) {
    return handle->GetRailHotImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetRailHotImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetRailHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetRailPushedImage(CScrollBarUI* handle) {
    return handle->GetRailPushedImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetRailPushedImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetRailPushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetRailDisabledImage(CScrollBarUI* handle) {
    return handle->GetRailDisabledImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetRailDisabledImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetRailDisabledImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetBkNormalImage(CScrollBarUI* handle) {
    return handle->GetBkNormalImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetBkNormalImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetBkNormalImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetBkHotImage(CScrollBarUI* handle) {
    return handle->GetBkHotImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetBkHotImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetBkHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetBkPushedImage(CScrollBarUI* handle) {
    return handle->GetBkPushedImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetBkPushedImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetBkPushedImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_ScrollBarUI_GetBkDisabledImage(CScrollBarUI* handle) {
    return handle->GetBkDisabledImage();
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetBkDisabledImage(CScrollBarUI* handle ,LPCTSTR pStrImage) {
    handle->SetBkDisabledImage(pStrImage);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetPos(CScrollBarUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_DoEvent(CScrollBarUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_SetAttribute(CScrollBarUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_DoPaint(CScrollBarUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_PaintBk(CScrollBarUI* handle ,HDC hDC) {
    handle->PaintBk(hDC);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_PaintButton1(CScrollBarUI* handle ,HDC hDC) {
    handle->PaintButton1(hDC);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_PaintButton2(CScrollBarUI* handle ,HDC hDC) {
    handle->PaintButton2(hDC);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_PaintThumb(CScrollBarUI* handle ,HDC hDC) {
    handle->PaintThumb(hDC);
}

DRIECTUILIB_API void Delphi_ScrollBarUI_PaintRail(CScrollBarUI* handle ,HDC hDC) {
    handle->PaintRail(hDC);
}

//================================CSliderUI============================

DRIECTUILIB_API CSliderUI* Delphi_SliderUI_CppCreate() {
    return new CSliderUI();
}

DRIECTUILIB_API void Delphi_SliderUI_CppDestroy(CSliderUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_SliderUI_GetClass(CSliderUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API UINT Delphi_SliderUI_GetControlFlags(CSliderUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API LPVOID Delphi_SliderUI_GetInterface(CSliderUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_SliderUI_SetEnabled(CSliderUI* handle ,bool bEnable) {
    handle->SetEnabled(bEnable);
}

DRIECTUILIB_API int Delphi_SliderUI_GetChangeStep(CSliderUI* handle) {
    return handle->GetChangeStep();
}

DRIECTUILIB_API void Delphi_SliderUI_SetChangeStep(CSliderUI* handle ,int step) {
    handle->SetChangeStep(step);
}

DRIECTUILIB_API void Delphi_SliderUI_SetThumbSize(CSliderUI* handle ,SIZE szXY) {
    handle->SetThumbSize(szXY);
}

DRIECTUILIB_API void Delphi_SliderUI_GetThumbRect(CSliderUI* handle, RECT& Result) {
    Result = handle->GetThumbRect();
}

DRIECTUILIB_API LPCTSTR Delphi_SliderUI_GetThumbImage(CSliderUI* handle) {
    return handle->GetThumbImage();
}

DRIECTUILIB_API void Delphi_SliderUI_SetThumbImage(CSliderUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_SliderUI_GetThumbHotImage(CSliderUI* handle) {
    return handle->GetThumbHotImage();
}

DRIECTUILIB_API void Delphi_SliderUI_SetThumbHotImage(CSliderUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbHotImage(pStrImage);
}

DRIECTUILIB_API LPCTSTR Delphi_SliderUI_GetThumbPushedImage(CSliderUI* handle) {
    return handle->GetThumbPushedImage();
}

DRIECTUILIB_API void Delphi_SliderUI_SetThumbPushedImage(CSliderUI* handle ,LPCTSTR pStrImage) {
    handle->SetThumbPushedImage(pStrImage);
}

DRIECTUILIB_API void Delphi_SliderUI_DoEvent(CSliderUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_SliderUI_SetAttribute(CSliderUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_SliderUI_PaintStatusImage(CSliderUI* handle ,HDC hDC) {
    handle->PaintStatusImage(hDC);
}

//================================CTextUI============================

DRIECTUILIB_API CTextUI* Delphi_TextUI_CppCreate() {
    return new CTextUI();
}

DRIECTUILIB_API void Delphi_TextUI_CppDestroy(CTextUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_TextUI_GetClass(CTextUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API UINT Delphi_TextUI_GetControlFlags(CTextUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API LPVOID Delphi_TextUI_GetInterface(CTextUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API CDuiString* Delphi_TextUI_GetLinkContent(CTextUI* handle ,int iIndex) {
    return handle->GetLinkContent(iIndex);
}

DRIECTUILIB_API void Delphi_TextUI_DoEvent(CTextUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_TextUI_EstimateSize(CTextUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_TextUI_PaintText(CTextUI* handle ,HDC hDC) {
    handle->PaintText(hDC);
}

//================================CTreeNodeUI============================

DRIECTUILIB_API CTreeNodeUI* Delphi_TreeNodeUI_CppCreate(CTreeNodeUI* _ParentNode) {
    return new CTreeNodeUI(_ParentNode);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_CppDestroy(CTreeNodeUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_TreeNodeUI_GetClass(CTreeNodeUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_TreeNodeUI_GetInterface(CTreeNodeUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_DoEvent(CTreeNodeUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_Invalidate(CTreeNodeUI* handle) {
    handle->Invalidate();
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_Select(CTreeNodeUI* handle ,bool bSelect) {
    return handle->Select(bSelect);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_Add(CTreeNodeUI* handle ,CControlUI* _pTreeNodeUI) {
    return handle->Add(_pTreeNodeUI);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_AddAt(CTreeNodeUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_Remove(CTreeNodeUI* handle ,CControlUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetVisibleTag(CTreeNodeUI* handle ,bool _IsVisible) {
    handle->SetVisibleTag(_IsVisible);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_GetVisibleTag(CTreeNodeUI* handle) {
    return handle->GetVisibleTag();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetItemText(CTreeNodeUI* handle ,LPCTSTR pstrValue) {
    handle->SetItemText(pstrValue);
}

DRIECTUILIB_API CDuiString Delphi_TreeNodeUI_GetItemText(CTreeNodeUI* handle) {
    return handle->GetItemText();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_CheckBoxSelected(CTreeNodeUI* handle ,bool _Selected) {
    handle->CheckBoxSelected(_Selected);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_IsCheckBoxSelected(CTreeNodeUI* handle) {
    return handle->IsCheckBoxSelected();
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_IsHasChild(CTreeNodeUI* handle) {
    return handle->IsHasChild();
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_AddChildNode(CTreeNodeUI* handle ,CTreeNodeUI* _pTreeNodeUI) {
    return handle->AddChildNode(_pTreeNodeUI);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_RemoveAt(CTreeNodeUI* handle ,CTreeNodeUI* _pTreeNodeUI) {
    return handle->RemoveAt(_pTreeNodeUI);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetParentNode(CTreeNodeUI* handle ,CTreeNodeUI* _pParentTreeNode) {
    handle->SetParentNode(_pParentTreeNode);
}

DRIECTUILIB_API CTreeNodeUI* Delphi_TreeNodeUI_GetParentNode(CTreeNodeUI* handle) {
    return handle->GetParentNode();
}

DRIECTUILIB_API long Delphi_TreeNodeUI_GetCountChild(CTreeNodeUI* handle) {
    return handle->GetCountChild();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetTreeView(CTreeNodeUI* handle ,CTreeViewUI* _CTreeViewUI) {
    handle->SetTreeView(_CTreeViewUI);
}

DRIECTUILIB_API CTreeViewUI* Delphi_TreeNodeUI_GetTreeView(CTreeNodeUI* handle) {
    return handle->GetTreeView();
}

DRIECTUILIB_API CTreeNodeUI* Delphi_TreeNodeUI_GetChildNode(CTreeNodeUI* handle ,int _nIndex) {
    return handle->GetChildNode(_nIndex);
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetVisibleFolderBtn(CTreeNodeUI* handle ,bool _IsVisibled) {
    handle->SetVisibleFolderBtn(_IsVisibled);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_GetVisibleFolderBtn(CTreeNodeUI* handle) {
    return handle->GetVisibleFolderBtn();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetVisibleCheckBtn(CTreeNodeUI* handle ,bool _IsVisibled) {
    handle->SetVisibleCheckBtn(_IsVisibled);
}

DRIECTUILIB_API bool Delphi_TreeNodeUI_GetVisibleCheckBtn(CTreeNodeUI* handle) {
    return handle->GetVisibleCheckBtn();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetItemTextColor(CTreeNodeUI* handle ,DWORD _dwItemTextColor) {
    handle->SetItemTextColor(_dwItemTextColor);
}

DRIECTUILIB_API DWORD Delphi_TreeNodeUI_GetItemTextColor(CTreeNodeUI* handle) {
    return handle->GetItemTextColor();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetItemHotTextColor(CTreeNodeUI* handle ,DWORD _dwItemHotTextColor) {
    handle->SetItemHotTextColor(_dwItemHotTextColor);
}

DRIECTUILIB_API DWORD Delphi_TreeNodeUI_GetItemHotTextColor(CTreeNodeUI* handle) {
    return handle->GetItemHotTextColor();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetSelItemTextColor(CTreeNodeUI* handle ,DWORD _dwSelItemTextColor) {
    handle->SetSelItemTextColor(_dwSelItemTextColor);
}

DRIECTUILIB_API DWORD Delphi_TreeNodeUI_GetSelItemTextColor(CTreeNodeUI* handle) {
    return handle->GetSelItemTextColor();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetSelItemHotTextColor(CTreeNodeUI* handle ,DWORD _dwSelHotItemTextColor) {
    handle->SetSelItemHotTextColor(_dwSelHotItemTextColor);
}

DRIECTUILIB_API DWORD Delphi_TreeNodeUI_GetSelItemHotTextColor(CTreeNodeUI* handle) {
    return handle->GetSelItemHotTextColor();
}

DRIECTUILIB_API void Delphi_TreeNodeUI_SetAttribute(CTreeNodeUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API CStdPtrArray Delphi_TreeNodeUI_GetTreeNodes(CTreeNodeUI* handle) {
    return handle->GetTreeNodes();
}

DRIECTUILIB_API int Delphi_TreeNodeUI_GetTreeIndex(CTreeNodeUI* handle) {
    return handle->GetTreeIndex();
}

DRIECTUILIB_API int Delphi_TreeNodeUI_GetNodeIndex(CTreeNodeUI* handle) {
    return handle->GetNodeIndex();
}

//================================CTreeViewUI============================

DRIECTUILIB_API CTreeViewUI* Delphi_TreeViewUI_CppCreate() {
    return new CTreeViewUI();
}

DRIECTUILIB_API void Delphi_TreeViewUI_CppDestroy(CTreeViewUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_TreeViewUI_GetClass(CTreeViewUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_TreeViewUI_GetInterface(CTreeViewUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_Add(CTreeViewUI* handle ,CTreeNodeUI* pControl) {
    return handle->Add(pControl);
}

DRIECTUILIB_API long Delphi_TreeViewUI_AddAt_01(CTreeViewUI* handle ,CTreeNodeUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_AddAt_02(CTreeViewUI* handle ,CTreeNodeUI* pControl, CTreeNodeUI* _IndexNode) {
    return handle->AddAt(pControl, _IndexNode);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_Remove(CTreeViewUI* handle ,CTreeNodeUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_RemoveAt(CTreeViewUI* handle ,int iIndex) {
    return handle->RemoveAt(iIndex);
}

DRIECTUILIB_API void Delphi_TreeViewUI_RemoveAll(CTreeViewUI* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API bool Delphi_TreeViewUI_OnCheckBoxChanged(CTreeViewUI* handle ,void* param) {
    return handle->OnCheckBoxChanged(param);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_OnFolderChanged(CTreeViewUI* handle ,void* param) {
    return handle->OnFolderChanged(param);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_OnDBClickItem(CTreeViewUI* handle ,void* param) {
    return handle->OnDBClickItem(param);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_SetItemCheckBox(CTreeViewUI* handle ,bool _Selected, CTreeNodeUI* _TreeNode) {
    return handle->SetItemCheckBox(_Selected, _TreeNode);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetItemExpand(CTreeViewUI* handle ,bool _Expanded, CTreeNodeUI* _TreeNode) {
    handle->SetItemExpand(_Expanded, _TreeNode);
}

DRIECTUILIB_API void Delphi_TreeViewUI_Notify(CTreeViewUI* handle ,TNotifyUI& msg) {
    handle->Notify(msg);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetVisibleFolderBtn(CTreeViewUI* handle ,bool _IsVisibled) {
    handle->SetVisibleFolderBtn(_IsVisibled);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_GetVisibleFolderBtn(CTreeViewUI* handle) {
    return handle->GetVisibleFolderBtn();
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetVisibleCheckBtn(CTreeViewUI* handle ,bool _IsVisibled) {
    handle->SetVisibleCheckBtn(_IsVisibled);
}

DRIECTUILIB_API bool Delphi_TreeViewUI_GetVisibleCheckBtn(CTreeViewUI* handle) {
    return handle->GetVisibleCheckBtn();
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetItemMinWidth(CTreeViewUI* handle ,UINT _ItemMinWidth) {
    handle->SetItemMinWidth(_ItemMinWidth);
}

DRIECTUILIB_API UINT Delphi_TreeViewUI_GetItemMinWidth(CTreeViewUI* handle) {
    return handle->GetItemMinWidth();
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetItemTextColor(CTreeViewUI* handle ,DWORD _dwItemTextColor) {
    handle->SetItemTextColor(_dwItemTextColor);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetItemHotTextColor(CTreeViewUI* handle ,DWORD _dwItemHotTextColor) {
    handle->SetItemHotTextColor(_dwItemHotTextColor);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetSelItemTextColor(CTreeViewUI* handle ,DWORD _dwSelItemTextColor) {
    handle->SetSelItemTextColor(_dwSelItemTextColor);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetSelItemHotTextColor(CTreeViewUI* handle ,DWORD _dwSelHotItemTextColor) {
    handle->SetSelItemHotTextColor(_dwSelHotItemTextColor);
}

DRIECTUILIB_API void Delphi_TreeViewUI_SetAttribute(CTreeViewUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

//================================CTabLayoutUI============================

DRIECTUILIB_API CTabLayoutUI* Delphi_TabLayoutUI_CppCreate() {
    return new CTabLayoutUI();
}

DRIECTUILIB_API void Delphi_TabLayoutUI_CppDestroy(CTabLayoutUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_TabLayoutUI_GetClass(CTabLayoutUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_TabLayoutUI_GetInterface(CTabLayoutUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API bool Delphi_TabLayoutUI_Add(CTabLayoutUI* handle ,CControlUI* pControl) {
    return handle->Add(pControl);
}

DRIECTUILIB_API bool Delphi_TabLayoutUI_AddAt(CTabLayoutUI* handle ,CControlUI* pControl, int iIndex) {
    return handle->AddAt(pControl, iIndex);
}

DRIECTUILIB_API bool Delphi_TabLayoutUI_Remove(CTabLayoutUI* handle ,CControlUI* pControl) {
    return handle->Remove(pControl);
}

DRIECTUILIB_API void Delphi_TabLayoutUI_RemoveAll(CTabLayoutUI* handle) {
    handle->RemoveAll();
}

DRIECTUILIB_API int Delphi_TabLayoutUI_GetCurSel(CTabLayoutUI* handle) {
    return handle->GetCurSel();
}

DRIECTUILIB_API bool Delphi_TabLayoutUI_SelectItem_01(CTabLayoutUI* handle ,int iIndex) {
    return handle->SelectItem(iIndex);
}

DRIECTUILIB_API bool Delphi_TabLayoutUI_SelectItem_02(CTabLayoutUI* handle ,CControlUI* pControl) {
    return handle->SelectItem(pControl);
}

DRIECTUILIB_API void Delphi_TabLayoutUI_SetPos(CTabLayoutUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_TabLayoutUI_SetAttribute(CTabLayoutUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

//================================CHorizontalLayoutUI============================

DRIECTUILIB_API CHorizontalLayoutUI* Delphi_HorizontalLayoutUI_CppCreate() {
    return new CHorizontalLayoutUI();
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_CppDestroy(CHorizontalLayoutUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_HorizontalLayoutUI_GetClass(CHorizontalLayoutUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_HorizontalLayoutUI_GetInterface(CHorizontalLayoutUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_HorizontalLayoutUI_GetControlFlags(CHorizontalLayoutUI* handle) {
    return handle->GetControlFlags();
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_SetSepWidth(CHorizontalLayoutUI* handle ,int iWidth) {
    handle->SetSepWidth(iWidth);
}

DRIECTUILIB_API int Delphi_HorizontalLayoutUI_GetSepWidth(CHorizontalLayoutUI* handle) {
    return handle->GetSepWidth();
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_SetSepImmMode(CHorizontalLayoutUI* handle ,bool bImmediately) {
    handle->SetSepImmMode(bImmediately);
}

DRIECTUILIB_API bool Delphi_HorizontalLayoutUI_IsSepImmMode(CHorizontalLayoutUI* handle) {
    return handle->IsSepImmMode();
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_SetAttribute(CHorizontalLayoutUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_DoEvent(CHorizontalLayoutUI* handle ,TEventUI& event) {
    handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_SetPos(CHorizontalLayoutUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_DoPostPaint(CHorizontalLayoutUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPostPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_HorizontalLayoutUI_GetThumbRect(CHorizontalLayoutUI* handle ,bool bUseNew, RECT& Result) {
    Result = handle->GetThumbRect(bUseNew);
}

//================================CListHeaderUI============================

DRIECTUILIB_API CListHeaderUI* Delphi_ListHeaderUI_CppCreate() {
    return new CListHeaderUI();
}

DRIECTUILIB_API void Delphi_ListHeaderUI_CppDestroy(CListHeaderUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ListHeaderUI_GetClass(CListHeaderUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ListHeaderUI_GetInterface(CListHeaderUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_ListHeaderUI_EstimateSize(CListHeaderUI* handle ,SIZE szAvailable, SIZE& Result) {
    Result = handle->EstimateSize(szAvailable);
}


//================================CActiveXUI============================

DRIECTUILIB_API CActiveXUI* Delphi_ActiveXUI_CppCreate() {
    return new CActiveXUI();
}

DRIECTUILIB_API void Delphi_ActiveXUI_CppDestroy(CActiveXUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ActiveXUI_GetClass(CActiveXUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ActiveXUI_GetInterface(CActiveXUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API HWND Delphi_ActiveXUI_GetHostWindow(CActiveXUI* handle) {
    return handle->GetHostWindow();
}

DRIECTUILIB_API bool Delphi_ActiveXUI_IsDelayCreate(CActiveXUI* handle) {
    return handle->IsDelayCreate();
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetDelayCreate(CActiveXUI* handle ,bool bDelayCreate) {
    handle->SetDelayCreate(bDelayCreate);
}

DRIECTUILIB_API bool Delphi_ActiveXUI_CreateControl_01(CActiveXUI* handle ,CLSID clsid) {
    return handle->CreateControl(clsid);
}

DRIECTUILIB_API bool Delphi_ActiveXUI_CreateControl_02(CActiveXUI* handle ,LPCTSTR pstrCLSID) {
    return handle->CreateControl(pstrCLSID);
}

DRIECTUILIB_API HRESULT Delphi_ActiveXUI_GetControl(CActiveXUI* handle ,IID iid, LPVOID* ppRet) {
    return handle->GetControl(iid, ppRet);
}

DRIECTUILIB_API void Delphi_ActiveXUI_GetClisd(CActiveXUI* handle, CLSID& Result) {
    Result = handle->GetClisd();
}

DRIECTUILIB_API CDuiString Delphi_ActiveXUI_GetModuleName(CActiveXUI* handle) {
    return handle->GetModuleName();
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetModuleName(CActiveXUI* handle ,LPCTSTR pstrText) {
    handle->SetModuleName(pstrText);
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetVisible(CActiveXUI* handle ,bool bVisible) {
    handle->SetVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetInternVisible(CActiveXUI* handle ,bool bVisible) {
    handle->SetInternVisible(bVisible);
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetPos(CActiveXUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ActiveXUI_Move(CActiveXUI* handle ,SIZE szOffset, bool bNeedInvalidate) {
    handle->Move(szOffset, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_ActiveXUI_DoPaint(CActiveXUI* handle ,HDC hDC, RECT& rcPaint) {
    handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ActiveXUI_SetAttribute(CActiveXUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

//================================CWebBrowserUI============================

DRIECTUILIB_API CWebBrowserUI* Delphi_WebBrowserUI_CppCreate() {
    return new CWebBrowserUI();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_CppDestroy(CWebBrowserUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_WebBrowserUI_GetClass(CWebBrowserUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_WebBrowserUI_GetInterface(CWebBrowserUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_WebBrowserUI_SetHomePage(CWebBrowserUI* handle ,LPCTSTR lpszUrl) {
    handle->SetHomePage(lpszUrl);
}

DRIECTUILIB_API LPCTSTR Delphi_WebBrowserUI_GetHomePage(CWebBrowserUI* handle) {
    return handle->GetHomePage();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_SetAutoNavigation(CWebBrowserUI* handle ,bool bAuto) {
    handle->SetAutoNavigation(bAuto);
}

DRIECTUILIB_API bool Delphi_WebBrowserUI_IsAutoNavigation(CWebBrowserUI* handle) {
    return handle->IsAutoNavigation();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_SetWebBrowserEventHandler(CWebBrowserUI* handle ,CWebBrowserEventHandler* pEventHandler) {
    handle->SetWebBrowserEventHandler(pEventHandler);
}

DRIECTUILIB_API void Delphi_WebBrowserUI_Navigate2(CWebBrowserUI* handle ,LPCTSTR lpszUrl) {
    handle->Navigate2(lpszUrl);
}

DRIECTUILIB_API void Delphi_WebBrowserUI_Refresh(CWebBrowserUI* handle) {
    handle->Refresh();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_Refresh2(CWebBrowserUI* handle ,int Level) {
    handle->Refresh2(Level);
}

DRIECTUILIB_API void Delphi_WebBrowserUI_GoBack(CWebBrowserUI* handle) {
    handle->GoBack();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_GoForward(CWebBrowserUI* handle) {
    handle->GoForward();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_NavigateHomePage(CWebBrowserUI* handle) {
    handle->NavigateHomePage();
}

DRIECTUILIB_API void Delphi_WebBrowserUI_NavigateUrl(CWebBrowserUI* handle ,LPCTSTR lpszUrl) {
    handle->NavigateUrl(lpszUrl);
}

DRIECTUILIB_API bool Delphi_WebBrowserUI_DoCreateControl(CWebBrowserUI* handle) {
    return handle->DoCreateControl();
}

DRIECTUILIB_API IWebBrowser2* Delphi_WebBrowserUI_GetWebBrowser2(CWebBrowserUI* handle) {
    return handle->GetWebBrowser2();
}

DRIECTUILIB_API IDispatch* Delphi_WebBrowserUI_GetHtmlWindow(CWebBrowserUI* handle) {
    return handle->GetHtmlWindow();
}

DRIECTUILIB_API DISPID Delphi_WebBrowserUI_FindId(IDispatch* pObj, LPOLESTR pName) {
    return CWebBrowserUI::FindId(pObj, pName);
}

DRIECTUILIB_API HRESULT Delphi_WebBrowserUI_InvokeMethod(IDispatch* pObj, LPOLESTR pMehtod, VARIANT* pVarResult, VARIANT* ps, int cArgs) {
    return CWebBrowserUI::InvokeMethod(pObj, pMehtod, pVarResult, ps, cArgs);
}

DRIECTUILIB_API HRESULT Delphi_WebBrowserUI_GetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue) {
    return CWebBrowserUI::GetProperty(pObj, pName, pValue);
}

DRIECTUILIB_API HRESULT Delphi_WebBrowserUI_SetProperty(IDispatch* pObj, LPOLESTR pName, VARIANT* pValue) {
    return CWebBrowserUI::SetProperty(pObj, pName, pValue);
}


//================================CRenderClip============================

DRIECTUILIB_API CRenderClip* Delphi_RenderClip_CppCreate() {
    return new CRenderClip();
}

DRIECTUILIB_API void Delphi_RenderClip_CppDestroy(CRenderClip* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_RenderClip_GenerateClip(HDC hDC, RECT rc, CRenderClip& clip) {
    CRenderClip::GenerateClip(hDC, rc, clip);
}

DRIECTUILIB_API void Delphi_RenderClip_GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip) {
    CRenderClip::GenerateRoundClip(hDC, rc, rcItem, width, height, clip);
}

DRIECTUILIB_API void Delphi_RenderClip_UseOldClipBegin(HDC hDC, CRenderClip& clip) {
    CRenderClip::UseOldClipBegin(hDC, clip);
}

DRIECTUILIB_API void Delphi_RenderClip_UseOldClipEnd(HDC hDC, CRenderClip& clip) {
    CRenderClip::UseOldClipEnd(hDC, clip);
}

//================================CRenderEngine============================

DRIECTUILIB_API CRenderEngine* Delphi_RenderEngine_CppCreate() {
    return new CRenderEngine();
}

DRIECTUILIB_API void Delphi_RenderEngine_CppDestroy(CRenderEngine* handle) {
    delete handle;
}

DRIECTUILIB_API DWORD Delphi_RenderEngine_AdjustColor(DWORD dwColor, short H, short S, short L) {
    return CRenderEngine::AdjustColor(dwColor, H, S, L);
}

DRIECTUILIB_API void Delphi_RenderEngine_AdjustImage(bool bUseHSL, TImageInfo* imageInfo, short H, short S, short L) {
    CRenderEngine::AdjustImage(bUseHSL, imageInfo, H, S, L);
}

DRIECTUILIB_API TImageInfo* Delphi_RenderEngine_LoadImage(STRINGorID bitmap, LPCTSTR type, DWORD mask) {
    return CRenderEngine::LoadImage(bitmap, type, mask);
}

DRIECTUILIB_API void Delphi_RenderEngine_FreeImage(TImageInfo* bitmap, bool bDelete) {
    CRenderEngine::FreeImage(bitmap, bDelete);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawImage_01(HDC hDC, HBITMAP hBitmap, RECT& rc, RECT& rcPaint, RECT& rcBmpPart, RECT& rcCorners, bool alphaChannel, BYTE uFade, bool hole, bool xtiled, bool ytiled) {
    CRenderEngine::DrawImage(hDC, hBitmap, rc, rcPaint, rcBmpPart, rcCorners, alphaChannel, uFade, hole, xtiled, ytiled);
}

DRIECTUILIB_API bool Delphi_RenderEngine_DrawImage_02(HDC hDC, CPaintManagerUI* pManager, RECT& rcItem, RECT& rcPaint, TDrawInfo& drawInfo) {
    return CRenderEngine::DrawImage(hDC, pManager, rcItem, rcPaint, drawInfo);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawColor(HDC hDC, RECT& rc, DWORD color) {
    CRenderEngine::DrawColor(hDC, rc, color);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawGradient(HDC hDC, RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) {
    CRenderEngine::DrawGradient(hDC, rc, dwFirst, dwSecond, bVertical, nSteps);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawLine(HDC hDC, RECT& rc, int nSize, DWORD dwPenColor, int nStyle) {
    CRenderEngine::DrawLine(hDC, rc, nSize, dwPenColor, nStyle);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawRect(HDC hDC, RECT& rc, int nSize, DWORD dwPenColor) {
    CRenderEngine::DrawRect(hDC, rc, nSize, dwPenColor);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawRoundRect(HDC hDC, RECT& rc, int width, int height, int nSize, DWORD dwPenColor) {
    CRenderEngine::DrawRoundRect(hDC, rc, width, height, nSize, dwPenColor);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) {
    CRenderEngine::DrawText(hDC, pManager, rc, pstrText, dwTextColor, iFont, uStyle);
}

DRIECTUILIB_API void Delphi_RenderEngine_DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, RECT* pLinks, CDuiString* sLinks, int& nLinkRects, UINT uStyle) {
    CRenderEngine::DrawHtmlText(hDC, pManager, rc, pstrText, dwTextColor, pLinks, sLinks, nLinkRects, uStyle);
}

DRIECTUILIB_API HBITMAP Delphi_RenderEngine_GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc) {
    return CRenderEngine::GenerateBitmap(pManager, pControl, rc);
}

DRIECTUILIB_API void Delphi_RenderEngine_GetTextSize(HDC hDC, CPaintManagerUI* pManager, LPCTSTR pstrText, int iFont, UINT uStyle, SIZE& Result) {
    Result = CRenderEngine::GetTextSize(hDC, pManager, pstrText, iFont, uStyle);
}


//================================CListElementUI============================

//DRIECTUILIB_API CListElementUI* Delphi_ListElementUI_CppCreate() {
//	return new CListElementUI();
//}

//DRIECTUILIB_API void Delphi_ListElementUI_CppDestroy(CListElementUI* handle) {
//	delete handle;
//}

DRIECTUILIB_API LPCTSTR Delphi_ListElementUI_GetClass(CListElementUI* handle) {
	return handle->GetClass();
}

DRIECTUILIB_API UINT Delphi_ListElementUI_GetControlFlags(CListElementUI* handle) {
	return handle->GetControlFlags();
}

DRIECTUILIB_API LPVOID Delphi_ListElementUI_GetInterface(CListElementUI* handle, LPCTSTR pstrName) {
	return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_ListElementUI_SetEnabled(CListElementUI* handle, bool bEnable) {
	handle->SetEnabled(bEnable);
}

DRIECTUILIB_API int Delphi_ListElementUI_GetIndex(CListElementUI* handle) {
	return handle->GetIndex();
}

DRIECTUILIB_API void Delphi_ListElementUI_SetIndex(CListElementUI* handle, int iIndex) {
	handle->SetIndex(iIndex);
}

DRIECTUILIB_API IListOwnerUI* Delphi_ListElementUI_GetOwner(CListElementUI* handle) {
	return handle->GetOwner();
}

DRIECTUILIB_API void Delphi_ListElementUI_SetOwner(CListElementUI* handle, CControlUI* pOwner) {
	handle->SetOwner(pOwner);
}

DRIECTUILIB_API void Delphi_ListElementUI_SetVisible(CListElementUI* handle, bool bVisible) {
	handle->SetVisible(bVisible);
}

DRIECTUILIB_API bool Delphi_ListElementUI_IsSelected(CListElementUI* handle) {
	return handle->IsSelected();
}

DRIECTUILIB_API bool Delphi_ListElementUI_Select(CListElementUI* handle, bool bSelect) {
	return handle->Select(bSelect);
}

DRIECTUILIB_API bool Delphi_ListElementUI_IsExpanded(CListElementUI* handle) {
	return handle->IsExpanded();
}

DRIECTUILIB_API bool Delphi_ListElementUI_Expand(CListElementUI* handle, bool bExpand) {
	return handle->Expand(bExpand);
}

DRIECTUILIB_API void Delphi_ListElementUI_Invalidate(CListElementUI* handle) {
	handle->Invalidate();
}

DRIECTUILIB_API bool Delphi_ListElementUI_Activate(CListElementUI* handle) {
	return handle->Activate();
}

DRIECTUILIB_API void Delphi_ListElementUI_DoEvent(CListElementUI* handle, TEventUI& event) {
	handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ListElementUI_SetAttribute(CListElementUI* handle, LPCTSTR pstrName, LPCTSTR pstrValue) {
	handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ListElementUI_DrawItemBk(CListElementUI* handle, HDC hDC, RECT& rcItem) {
	handle->DrawItemBk(hDC, rcItem);
}

//================================CListLabelElementUI============================

DRIECTUILIB_API CListLabelElementUI* Delphi_ListLabelElementUI_CppCreate() {
	return new CListLabelElementUI();
}

DRIECTUILIB_API void Delphi_ListLabelElementUI_CppDestroy(CListLabelElementUI* handle) {
	delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ListLabelElementUI_GetClass(CListLabelElementUI* handle) {
	return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ListLabelElementUI_GetInterface(CListLabelElementUI* handle, LPCTSTR pstrName) {
	return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_ListLabelElementUI_DoEvent(CListLabelElementUI* handle, TEventUI& event) {
	handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ListLabelElementUI_EstimateSize(CListLabelElementUI* handle, SIZE szAvailable, SIZE& Result) {
	Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_ListLabelElementUI_DoPaint(CListLabelElementUI* handle, HDC hDC, RECT& rcPaint) {
	handle->DoPaint(hDC, rcPaint);
}

DRIECTUILIB_API void Delphi_ListLabelElementUI_DrawItemText(CListLabelElementUI* handle, HDC hDC, RECT& rcItem) {
	handle->DrawItemText(hDC, rcItem);
}

//================================CListTextElementUI============================

DRIECTUILIB_API CListTextElementUI* Delphi_ListTextElementUI_CppCreate() {
	return new CListTextElementUI();
}

DRIECTUILIB_API void Delphi_ListTextElementUI_CppDestroy(CListTextElementUI* handle) {
	delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_ListTextElementUI_GetClass(CListTextElementUI* handle) {
	return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_ListTextElementUI_GetInterface(CListTextElementUI* handle, LPCTSTR pstrName) {
	return handle->GetInterface(pstrName);
}

DRIECTUILIB_API UINT Delphi_ListTextElementUI_GetControlFlags(CListTextElementUI* handle) {
	return handle->GetControlFlags();
}

DRIECTUILIB_API LPCTSTR Delphi_ListTextElementUI_GetText(CListTextElementUI* handle, int iIndex) {
	return handle->GetText(iIndex);
}

DRIECTUILIB_API void Delphi_ListTextElementUI_SetText(CListTextElementUI* handle, int iIndex, LPCTSTR pstrText) {
	handle->SetText(iIndex, pstrText);
}

DRIECTUILIB_API void Delphi_ListTextElementUI_SetOwner(CListTextElementUI* handle, CControlUI* pOwner) {
	handle->SetOwner(pOwner);
}

DRIECTUILIB_API CDuiString* Delphi_ListTextElementUI_GetLinkContent(CListTextElementUI* handle, int iIndex) {
	return handle->GetLinkContent(iIndex);
}

DRIECTUILIB_API void Delphi_ListTextElementUI_DoEvent(CListTextElementUI* handle, TEventUI& event) {
	handle->DoEvent(event);
}

DRIECTUILIB_API void Delphi_ListTextElementUI_EstimateSize(CListTextElementUI* handle, SIZE szAvailable, SIZE& Result) {
	Result = handle->EstimateSize(szAvailable);
}

DRIECTUILIB_API void Delphi_ListTextElementUI_DrawItemText(CListTextElementUI* handle, HDC hDC, RECT& rcItem) {
	handle->DrawItemText(hDC, rcItem);
}


//================================CChildLayoutUI============================

DRIECTUILIB_API CChildLayoutUI* Delphi_ChildLayoutUI_CppCreate() {
    return new CChildLayoutUI();
}

DRIECTUILIB_API void Delphi_ChildLayoutUI_CppDestroy(CChildLayoutUI* handle) {
    delete handle;
}

DRIECTUILIB_API void Delphi_ChildLayoutUI_Init(CChildLayoutUI* handle) {
    handle->Init();
}

DRIECTUILIB_API void Delphi_ChildLayoutUI_SetAttribute(CChildLayoutUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}

DRIECTUILIB_API void Delphi_ChildLayoutUI_SetChildLayoutXML(CChildLayoutUI* handle ,CDuiString pXML) {
    handle->SetChildLayoutXML(pXML);
}

DRIECTUILIB_API CDuiString Delphi_ChildLayoutUI_GetChildLayoutXML(CChildLayoutUI* handle) {
    return handle->GetChildLayoutXML();
}

DRIECTUILIB_API LPVOID Delphi_ChildLayoutUI_GetInterface(CChildLayoutUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API LPCTSTR Delphi_ChildLayoutUI_GetClass(CChildLayoutUI* handle) {
    return handle->GetClass();
}

//================================CTileLayoutUI============================

DRIECTUILIB_API CTileLayoutUI* Delphi_TileLayoutUI_CppCreate() {
    return new CTileLayoutUI();
}

DRIECTUILIB_API void Delphi_TileLayoutUI_CppDestroy(CTileLayoutUI* handle) {
    delete handle;
}

DRIECTUILIB_API LPCTSTR Delphi_TileLayoutUI_GetClass(CTileLayoutUI* handle) {
    return handle->GetClass();
}

DRIECTUILIB_API LPVOID Delphi_TileLayoutUI_GetInterface(CTileLayoutUI* handle ,LPCTSTR pstrName) {
    return handle->GetInterface(pstrName);
}

DRIECTUILIB_API void Delphi_TileLayoutUI_SetPos(CTileLayoutUI* handle ,RECT rc, bool bNeedInvalidate) {
    handle->SetPos(rc, bNeedInvalidate);
}

DRIECTUILIB_API void Delphi_TileLayoutUI_GetItemSize(CTileLayoutUI* handle, SIZE& Result) {
    Result = handle->GetItemSize();
}

DRIECTUILIB_API void Delphi_TileLayoutUI_SetItemSize(CTileLayoutUI* handle ,SIZE szItem) {
    handle->SetItemSize(szItem);
}

DRIECTUILIB_API int Delphi_TileLayoutUI_GetColumns(CTileLayoutUI* handle) {
    return handle->GetColumns();
}

DRIECTUILIB_API void Delphi_TileLayoutUI_SetColumns(CTileLayoutUI* handle ,int nCols) {
    handle->SetColumns(nCols);
}

DRIECTUILIB_API void Delphi_TileLayoutUI_SetAttribute(CTileLayoutUI* handle ,LPCTSTR pstrName, LPCTSTR pstrValue) {
    handle->SetAttribute(pstrName, pstrValue);
}


