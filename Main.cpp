#include "DxLib.h"
#include <stdlib.h>
#include <time.h>
#include "Input.h"
#include <vector>
#include <queue>

namespace
{
	const int BOX_SIZE = 50;

	const int MAZE_WIDTH = 27; //ウィンドウの横＝1350
	const int MAZE_HEIGHT = 17; //ウィンドウの縦＝850

	const int WIN_WIDTH = MAZE_WIDTH * BOX_SIZE;
	const int WIN_HEIGHT = MAZE_HEIGHT * BOX_SIZE;

	const int WALL = 0;   // 壁
	const int ROAD = 1;   // 通路

	const int BOX_COLOR = GetColor(0, 0, 0); // 壁の色（黒）
	const int ROAD_COLOR = GetColor(255, 255, 255); // 通路の色（白）

	int maze[MAZE_HEIGHT][MAZE_WIDTH];
}

struct Point
{
	int x, y;
};

void DxInit()
{
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText("迷路生成");
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(255, 255, 255);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		DxLib_End();
	}

	SetDrawScreen(DX_SCREEN_BACK);
	srand((unsigned int)time(NULL));

	SetDrawScreen(DX_SCREEN_BACK);

	Input::Initialize(GetMainWindowHandle());
}

void generateMaze(int x, int y)
{
	maze[y][x] = ROAD;

	const int directions[4][2] = {
		{0, -2},  // 上
		{0, 2},   // 下
		{-2, 0},  // 左
		{2, 0}    // 右
	};

	int order[4] = { 0, 1, 2, 3 };

	for (int i = 3; i > 0; i--) 
	{
		int j = rand() % (i + 1);
		int temp = order[i];
		order[i] = order[j];
		order[j] = temp;
	}


	for (int i = 0; i < 4; i++)
	{
		int dir_index = order[i];

		int dx = directions[dir_index][0];
		int dy = directions[dir_index][1];

		int next_x = x + dx;
		int next_y = y + dy;

		if (next_x > 0 && next_x < MAZE_WIDTH - 1 &&
			next_y > 0 && next_y < MAZE_HEIGHT - 1 &&
			maze[next_y][next_x] == WALL)
		{
			maze[y + dy / 2][x + dx / 2] = ROAD;

			generateMaze(next_x, next_y);
		}
	}
}

void setupMaze()
{
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			maze[y][x] = WALL;
		}
	}
	generateMaze(1, 1);
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	DxInit();

	setupMaze();

	while (true)
	{
		ClearDrawScreen();
		Input::Update();

		//ここに処理を書く
		if (Input::IsKeyDown(DIK_SPACE))
		{
			setupMaze();
		}
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			for (int x = 0; x < MAZE_WIDTH; x++)
			{
				int color;
				if (maze[y][x] == WALL)
				{
					color = BOX_COLOR;
				}
				else 
				{
					color = ROAD_COLOR;
				}
				int draw_x = x * BOX_SIZE;
				int draw_y = y * BOX_SIZE;

				DrawBox(draw_x, draw_y, draw_x + BOX_SIZE, draw_y + BOX_SIZE, color, TRUE);
			}
		}

		ScreenFlip();
		WaitTimer(16);

		if (ProcessMessage() == -1)
			break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
	}

	DxLib_End();
	return 0;
}