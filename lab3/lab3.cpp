// lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <queue>
#include <iostream>
#include <random>
#include <Windows.h>

using namespace std;
CRITICAL_SECTION CS;
queue<int> numbers;
const int NUMBER_COUNT = 100;
volatile bool GeneratorCompleted = FALSE;

// ��������������� ���������� �������
void GenerateNumbers();
void ProcessNumbers(int*);
DWORD WINAPI ThreadProc1(LPVOID lpParameter)
{
	GenerateNumbers();
	return 0;
}
DWORD WINAPI ThreadProc2(LPVOID lpParameter)
{
	int sum;
	ProcessNumbers(&sum);
	return 0;
}
// ����� �����
int _tmain(int argc, _TCHAR* argv[])
{	
	//int sum;

	// ����� ���������� ��������� ����� (��� �������������).
	//srand(200);

	//GenerateNumbers();
	//ProcessNumbers(&sum);
	InitializeCriticalSection (&CS);
	HANDLE hThreads[3];
	
	hThreads[0] = CreateThread(NULL, NULL, ThreadProc1, NULL, 0, 0);
	hThreads[1] = CreateThread(NULL, NULL, ThreadProc2, NULL, 0, 0);
	hThreads[2] = CreateThread(NULL, NULL, ThreadProc2, NULL, 0, 0);
	WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
	return 0;
}

void GenerateNumbers()
{
	for (size_t i = 0; i < NUMBER_COUNT; i++)
	{
		EnterCriticalSection (&CS);
		// ������� � ������� ��������� ����� 0-99
		numbers.push(rand()%100);
		cout << "Generated number " << numbers.back() << endl;
		// ��� ��� ������� (�������� ������ ������)
		LeaveCriticalSection (&CS);
		Sleep(2000);
		
	}
	GeneratorCompleted = TRUE;
}

void ProcessNumbers(int* sumAddress)
{
	while (!GeneratorCompleted)
	{
		EnterCriticalSection (&CS);
		if (numbers.size() != 0)
		{
			
			// �������� ��������� ������� �� �������
			int current = numbers.front();
			cout << "Processing element " << current << endl;
			// ������ ��� �� �������
			numbers.pop();
			// �������� � ����������, ������������� �����
			*sumAddress += current;
			// ��� ��� ������� (�������� ������ ������)
		}
			LeaveCriticalSection (&CS);
			Sleep(500);
			
		}
	}

// ��������� � ���, ��� �������� ������� - ���� ������
/*DWORD WINAPI ThreadBody(LPVOID context)
{
	return 0;
}*/