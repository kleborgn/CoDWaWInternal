// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include <iostream>

#define NOCLIP_ADDRESS 0x018EF2A4
#define HEALTH_ADDRESS 0x176C8B8
#define MONEY_OFFSET 0x014EF124

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole(); //attaches console
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);	//sets cout to be used with our newly created console
	std::cout << "Injected !" << std::endl;
	std::cout << "F1 : No Clip (on/off)" << std::endl;
	std::cout << "F2 : God Mode/Health Loop (on/off)" << std::endl;
	std::cout << "F3 : Max Money" << std::endl;
	std::cout << "INSERT : Uninject" << std::endl;

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"CodWaw.exe");

	//calling it with NULL also gives you the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	bool bNoClip = false, bGodMode = false;

	while (true)
	{
		if (GetAsyncKeyState(VK_F1)) {
			bNoClip = !bNoClip;

			if (bNoClip)
			{
				std::cout << "NoClip ON" << std::endl;
				*(BYTE*)(NOCLIP_ADDRESS) = 1;
			}
			else {
				std::cout << "NoClip OFF" << std::endl;
				*(BYTE*)(NOCLIP_ADDRESS) = 0;
			}
		}

		if (GetAsyncKeyState(VK_F2))
		{
			bGodMode = !bGodMode;

			if (bGodMode)
			{
				std::cout << "GodMode ON" << std::endl;
			}
			else {
				std::cout << "GodMode OFF" << std::endl;
			}
		}

		if (GetAsyncKeyState(VK_F3))
		{
			*(int*)(moduleBase + MONEY_OFFSET) = 300000;
		}

		if (GetAsyncKeyState(VK_INSERT))
		{
			std::cout << "Uninjecting..." << std::endl;
			break;
		}

		if (bGodMode)
		{
			*(int*)(HEALTH_ADDRESS) = 1337;
		}


		Sleep(100);
	}

	// Hook
	
	//

	//cleanup
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

