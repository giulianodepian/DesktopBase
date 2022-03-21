#pragma comment (lib, "comctl32")
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#include <string>;

const wchar_t className[] = L"Desktop Base";
const wchar_t windowTitle[] = L"Desktop Base";
const int MARGENTABCONTROLRIGHT = 10;
const int MARGENTABCONTROLBOTTOM = 25;
const int MARGENTABDISPLAYRIGHT = 0;
const int MARGENTABDISPLAYBOTTOM = 21;
const int TEXTTABWIDTH = 300;
const int TEXTTABHEIGHT = 20;
const int TEXTMARGENLEFT = 10;
const int TEXTMARGENTOP = 10;
const int BUTTONWIDTH = 100;
const int BUTTONHEIGHT = 20;
const int BUTTONMARGENLEFT = 10;
const int BUTTONMARGENTOP = 10;
const int BUTTONCALCULAR_ID = 300;
const int BUTTONADD_ID = 301;
const int TEXTSLIMIT = 10;

HWND hwndTab;
HWND hwndDisplay;
HWND hwndText[TEXTSLIMIT];
HWND hwndButtonCalcular;
HWND hwndButtonAdd;

int cantText = 0;

RECT GetChildWinRelativeCoordinates(HWND hwndChild) {
	RECT rcChild;
	GetWindowRect(hwndChild, &rcChild);
	MapWindowPoints(HWND_DESKTOP, GetParent(hwndTab), (LPPOINT)&rcChild, 2); //el primer parametro indica relativas a qué son las coordenadas a transformar
	return rcChild;
}

VOID IntToWchar(int total, wchar_t* bufferInput) {
	int i = 0;
	wchar_t tempArray[1024];
	while (total > 0) {
		tempArray[i] = (total % 10) + 48;
		total = total / 10;
		i++;
	}
	i--;
	int e = 0;
	while (i >= 0) {
		bufferInput[e] = tempArray[i];
		i--;
		e++;
	}
	bufferInput[e] = '\0';
}

HWND CreateTabControl(HWND parentHWND) {
	RECT rcClient;
	INITCOMMONCONTROLSEX icex;
	TCITEM tabAttr;
	TCHAR tabTextBuffer[256];

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	GetClientRect(parentHWND, &rcClient);
	HWND hwndTab = CreateWindowEx(
		0,
		WC_TABCONTROL,
		L"",
		WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE,
		0, 0, rcClient.right - MARGENTABCONTROLRIGHT, rcClient.bottom - MARGENTABCONTROLBOTTOM,
		parentHWND,
		NULL,
		(HINSTANCE)GetWindowLongPtr(parentHWND, GWLP_HINSTANCE),
		NULL);
	if (hwndTab == NULL) {
		return NULL;
	}
	tabAttr.mask = TCIF_TEXT;
	tabAttr.pszText = tabTextBuffer;
	LoadString((HINSTANCE)GetWindowLongPtr(parentHWND, GWLP_HINSTANCE), IDS_STRING101, tabTextBuffer, sizeof(tabTextBuffer) / sizeof(TCHAR));
	TabCtrl_InsertItem(hwndTab, 0, &tabAttr);
	LoadString((HINSTANCE)GetWindowLongPtr(parentHWND, GWLP_HINSTANCE), IDS_STRING104, tabTextBuffer, sizeof(tabTextBuffer) / sizeof(TCHAR));
	TabCtrl_InsertItem(hwndTab, 1, &tabAttr);
	return hwndTab;
}

HWND CreateDisplayWindow(HWND hwndTab) {
	RECT rcTab;
	rcTab = GetChildWinRelativeCoordinates(hwndTab);
	TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
	HWND hwndStatic = CreateWindowEx(0,
		WC_STATIC,
		L"",
		WS_CHILD | WS_VISIBLE,
		rcTab.left, rcTab.top, rcTab.right - MARGENTABDISPLAYRIGHT, rcTab.bottom - MARGENTABDISPLAYBOTTOM,
		hwndTab,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwndTab, GWLP_HINSTANCE),
		NULL);
	return hwndStatic;
}

HWND CreateEditTextWindow(HWND hwndDisplay) {
	RECT rcTab;
	RECT rcText;
	rcTab = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
	if (cantText == 0) {
		rcText = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
	}
	else {
		rcText = GetChildWinRelativeCoordinates(hwndText[cantText - 1]);
	}
	TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
	HWND hwndText = CreateWindowEx(0,
		WC_EDIT,
		L"",
		WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_LEFT,
		rcTab.left + TEXTMARGENLEFT, rcText.top + TEXTMARGENTOP, TEXTTABWIDTH, TEXTTABHEIGHT,
		hwndDisplay,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwndDisplay, GWLP_HINSTANCE),
		NULL);
	return hwndText;
}

HWND CreateButtonCalcularWindow(HWND hwndDisplay) {
	RECT rcTab;
	RECT rcText;
	rcTab = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
	rcText = GetChildWinRelativeCoordinates(hwndText[0]);
	TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
	HWND hwndText = CreateWindowEx(0,
		WC_BUTTON,
		L"Calcular",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rcTab.left + BUTTONMARGENLEFT, rcText.bottom, BUTTONWIDTH, BUTTONHEIGHT,
		hwndDisplay,
		(HMENU)BUTTONCALCULAR_ID,
		(HINSTANCE)GetWindowLongPtr(hwndDisplay, GWLP_HINSTANCE),
		NULL);
	return hwndText;
}

HWND CreateButtonAddWindow(HWND hwndDisplay) {
	RECT rcTab;
	RECT rcText;
	rcTab = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
	rcText = GetChildWinRelativeCoordinates(hwndText[0]);
	TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
	HWND hwndText = CreateWindowEx(0,
		WC_BUTTON,
		L"Agregar",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		rcTab.left + BUTTONMARGENLEFT * 2 + BUTTONWIDTH, rcText.bottom, BUTTONWIDTH, BUTTONHEIGHT,
		hwndDisplay,
		(HMENU)BUTTONADD_ID,
		(HINSTANCE)GetWindowLongPtr(hwndDisplay, GWLP_HINSTANCE),
		NULL);
	return hwndText;
}


BOOL OnNotify(HWND hwndTab, HWND hwndDisplay, HWND hwndText[], HWND hwndButtonCalcular, LPARAM lParam) {

	switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGING:
		{
			// Return FALSE to allow the selection to change.
			return FALSE;
		}
		case TCN_SELCHANGE: {
			int idSelectedTab = TabCtrl_GetCurSel(hwndTab);
			if (idSelectedTab == 0) {
				for (int i = 1; i <= cantText; i++) {
					ShowWindow(hwndText[i - 1], SW_NORMAL);
				}
				ShowWindow(hwndButtonCalcular, SW_NORMAL);
				ShowWindow(hwndButtonAdd, SW_NORMAL);
			}
			else {
				for (int i = 1; i <= cantText; i++) {
					ShowWindow(hwndText[i-1], SW_HIDE);
				}
				ShowWindow(hwndButtonCalcular, SW_HIDE);
				ShowWindow(hwndButtonAdd, SW_HIDE);
			}
		}
	}
	return 0;
}

BOOL OnSize(HWND hwndDisplay) {
	RECT rcClient;
	GetClientRect(GetParent(GetParent(hwndDisplay)), &rcClient);
	SetWindowPos(GetParent(hwndDisplay), HWND_TOP,
		rcClient.left, rcClient.top, rcClient.right - MARGENTABCONTROLRIGHT, rcClient.bottom - MARGENTABCONTROLBOTTOM,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	RECT rcTab = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
	TabCtrl_AdjustRect(GetParent(hwndDisplay), FALSE, &rcTab);
	SetWindowPos(hwndDisplay, HWND_TOP,
		rcTab.left, rcTab.top, rcTab.right - MARGENTABDISPLAYRIGHT, rcTab.bottom - MARGENTABDISPLAYBOTTOM,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	UpdateWindow(hwndDisplay);
	return 0;
}

BOOL OnPress(HWND hwndButtonPressed, WPARAM wParam) {
	switch(LOWORD(wParam)) {
		case BUTTONCALCULAR_ID: {
			int total = 0;
			wchar_t bufferInput[1024];
			for (int i = 1; i <= cantText; i++) {
				GetWindowText(hwndText[i-1], bufferInput, sizeof(bufferInput) / sizeof(bufferInput[0]));
				if (bufferInput[0] != '\0') {
					total += std::stoi(bufferInput);
				}
			}
			if (total != 0) {
				IntToWchar(total, bufferInput);
			}
			MessageBox(NULL, bufferInput, L"Total", MB_OK);

		}
			break;
		case BUTTONADD_ID: {
			if (cantText < TEXTSLIMIT) {
				hwndText[cantText] = CreateEditTextWindow(hwndDisplay);
				cantText++;
				RECT rcLastText;
				RECT rcTab;
				rcTab = GetChildWinRelativeCoordinates(GetParent(hwndDisplay));
				TabCtrl_AdjustRect(hwndTab, FALSE, &rcTab);
				rcLastText = GetChildWinRelativeCoordinates(hwndText[cantText - 1]);
				SetWindowPos(hwndButtonCalcular, HWND_TOP,
					rcTab.left + BUTTONMARGENLEFT, rcLastText.bottom, BUTTONWIDTH, BUTTONHEIGHT,
					SWP_NOZORDER | SWP_SHOWWINDOW);
				SetWindowPos(hwndButtonAdd, HWND_TOP,
					rcTab.left + BUTTONMARGENLEFT * 2 + BUTTONWIDTH, rcLastText.bottom, BUTTONWIDTH, BUTTONHEIGHT,
					SWP_NOZORDER | SWP_SHOWWINDOW);
				UpdateWindow(hwndButtonCalcular);
				UpdateWindow(hwndButtonAdd);
			}
			else {
				MessageBox(NULL, L"No se pueden crear mas editores de texto", L"Error", MB_OK);
			}
		}
			break;
	}
	return 0;
}

LRESULT CALLBACK DisplayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	switch (uMsg) {
		case WM_COMMAND: {
			OnPress(hwnd, wParam);
		}
			break;
		case WM_NCDESTROY: {
			RemoveWindowSubclass(hwnd, DisplayProc, 0);
		}
		default: {
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
			break;
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE: {
			hwndTab = CreateTabControl(hwnd);
			hwndDisplay = CreateDisplayWindow(hwndTab);
			hwndText[cantText] = CreateEditTextWindow(hwndDisplay);
			cantText++;
			hwndButtonCalcular = CreateButtonCalcularWindow(hwndDisplay);
			hwndButtonAdd = CreateButtonAddWindow(hwndDisplay);
			SetWindowSubclass(hwndDisplay, DisplayProc, 0, 0);
		}
			break;
		case WM_NOTIFY: {
			OnNotify(hwndTab, hwndDisplay, hwndText, hwndButtonCalcular, lParam);
		}
			break;

		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hwnd, &ps);
		}
			break;
		
		case WM_SIZE: {
			OnSize(hwndDisplay);
		}
			break;
		case WM_CLOSE: {
			if (MessageBox(hwnd, L"Quieres cerrar la aplicacion?", windowTitle, MB_OKCANCEL) == IDOK) {
				DestroyWindow(hwnd);
			}
		}
			break;
		case WM_DESTROY: {
			PostQuitMessage(0);
		}
			break;
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
			break;
		}
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			L"Call to RegisterClassEx failed!",
			NULL,
			MB_OK | MB_ICONERROR);

		return 1;
	}

	HWND hwnd = CreateWindowEx(
		0,
		className,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

