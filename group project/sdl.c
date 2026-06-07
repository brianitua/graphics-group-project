#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file);
    if (!texture)
        printf("Failed to load image '%s': %s\n", file, SDL_GetError());
    return texture;
}

void DrawBrickWall(SDL_Renderer* renderer, int winW, int winH)
{
    int brickW = 80, brickH = 30, gap = 4;

    for (int row = 0; row * (brickH + gap) < winH; row++)
    {
        int offsetX = (row % 2 == 0) ? 0 : -(brickW / 2);
        for (int col = -1; col * (brickW + gap) + offsetX < winW; col++)
        {
            int x = col * (brickW + gap) + offsetX;
            int y = row * (brickH + gap);

            // Changed to a cool slate/greyish-blue color scheme
            int shade = 100 + (row * 5 + col * 4) % 40;
            SDL_SetRenderDrawColor(renderer, shade / 2, shade / 1.5, shade, 255);
            SDL_FRect brick = { (float)x, (float)y, (float)brickW, (float)brickH };
            SDL_RenderFillRect(renderer, &brick);

            // Brick highlight layer
            SDL_SetRenderDrawColor(renderer, shade / 2 + 20, shade / 1.5 + 20, shade + 20, 255);
            SDL_FRect top = { (float)x, (float)y, (float)brickW, 2 };
            SDL_FRect left = { (float)x, (float)y, 2, (float)brickH };
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &left);
        }
    }
}

// Renamed and recolored to a Silver/Chrome aesthetic
void DrawSilverFrame(SDL_Renderer* renderer, SDL_Texture* texture,
    float x, float y, float w, float h)
{
    // Drop shadow (slightly cooler and darker)
    SDL_SetRenderDrawColor(renderer, 20, 20, 25, 150);
    SDL_FRect shadow = { x + 8, y + 8, w, h };
    SDL_RenderFillRect(renderer, &shadow);

    // Outer silver border
    SDL_SetRenderDrawColor(renderer, 150, 150, 160, 255);
    SDL_FRect outer = { x, y, w, h };
    SDL_RenderFillRect(renderer, &outer);

    // Silver highlight layer
    SDL_SetRenderDrawColor(renderer, 230, 230, 240, 255);
    SDL_FRect highlight = { x + 4, y + 4, w - 8, h - 8 };
    SDL_RenderFillRect(renderer, &highlight);

    // Dark silver inner edge
    SDL_SetRenderDrawColor(renderer, 90, 90, 100, 255);
    SDL_FRect inner = { x + 10, y + 10, w - 20, h - 20 };
    SDL_RenderFillRect(renderer, &inner);

    // Image fills the frame completely (no matte)
    SDL_FRect image = { x + 14, y + 14, w - 28, h - 28 };
    SDL_RenderTexture(renderer, texture, NULL, &image);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    int winW = 1200, winH = 750;
    SDL_Window* window = SDL_CreateWindow("Art Gallery", winW, winH, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // Updated paths to use the exact files you uploaded
    SDL_Texture* img1 = LoadTexture(renderer, "TheRoom.png");
    SDL_Texture* img2 = LoadTexture(renderer, "GarageWorkshop.png");
    SDL_Texture* img3 = LoadTexture(renderer, "Assignment.jpeg");

    // Frame sizes
    float frameW = 340, frameH = 240;
    float smallW = 300, smallH = 220;

    // Top row - centered together
    float totalTopW = frameW + frameW + 60; // two frames + gap
    float topStartX = (winW - totalTopW) / 2.0f;
    float topY = 60;

    float x1 = topStartX;
    float x2 = topStartX + frameW + 60;

    // Bottom frame - centered
    float x3 = (winW - smallW) / 2.0f;
    float bottomY = topY + frameH + 60;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                running = false;

        // Render the new blue/slate wall
        DrawBrickWall(renderer, winW, winH);

        // Render the new silver frames
        DrawSilverFrame(renderer, img1, x1, topY, frameW, frameH);
        DrawSilverFrame(renderer, img2, x2, topY, frameW, frameH);
        DrawSilverFrame(renderer, img3, x3, bottomY, smallW, smallH);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(img1);
    SDL_DestroyTexture(img2);
    SDL_DestroyTexture(img3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}