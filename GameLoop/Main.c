#include <stdio.h>
#include <SDL.h>
#include "./constants.h"

int running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct Rect {
	float x;
	float y;
	float width;
	float height;
} rect;


int last_frame_time = 0;


int init_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(
		window, 
		-1, // Default rendering driver
		0 // No flags
	);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return FALSE;
	}

	return TRUE;
}


void setup(void) {
	rect.x = 20;
	rect.y = 20;
	rect.width = 15;
	rect.height = 15;
}


void destroy_window(void) {
	// Teardown in reverse order:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		running = FALSE;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) running = FALSE;
		break;
	}
} 


void update() {
	// Lock execution until we reach target frametime
	//while (
	//	!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + TARGET_FRAMETIME)
	//);
	
	// Without spinlock (releasing execution):
	int wait = TARGET_FRAMETIME - (SDL_GetTicks() - last_frame_time);
	if ((wait > 0) && (wait <= TARGET_FRAMETIME)) SDL_Delay(wait);

	// Ticks from last frame converted to seconds:
	// Objects should update as a function of pixels per second,
	// rather than a function of pixels per frame. 
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks(); // Ticks since SDL_Init()

	////////////////////////////////////////////////////////////////////////
	// Game objects updated below
	////////////////////////////////////////////////////////////////////////

	rect.x += 50 * delta_time;
	rect.y += 80 * delta_time;
}


void draw() {
	SDL_SetRenderDrawColor(renderer, 50,50,50,255);
	SDL_RenderClear(renderer);

	SDL_Rect square = {
		(int)rect.x,
		(int)rect.y,
		(int)rect.width,
		(int)rect.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &square);

	SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
	running = init_window();

	setup();
	while (running) {
		process_input();
		update();
		draw();
	}
	destroy_window();

	return 0;
}
