#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0x49,0x8A,0x55,0x3D,0xB4,0x60,0x40,0x0A,0xB0,0x54,0xC0,0xA5,0xBA,0x38,0x32,0x26}
#define BUFSIZE 16

PBL_APP_INFO(MY_UUID,
		"Minitime", "John Weachock",
		1, 0,
		DEFAULT_MENU_ICON,
		APP_INFO_WATCH_FACE);

Window window;
TextLayer time_layer, date_layer;
char time_txt[BUFSIZE], date_txt[BUFSIZE];

void tick() {
	PblTm current_time;

	get_time(&current_time);

	int hour = current_time.tm_hour % 12;
	if(hour == 0) {
		hour = 12;
	}

	snprintf(time_txt, BUFSIZE, "%d:%02d",
			hour,
			current_time.tm_min);

	snprintf(date_txt, BUFSIZE, "%d/%d/%02d",
			current_time.tm_mon+1,
			current_time.tm_mday,
			current_time.tm_year%100);

	text_layer_set_text(&time_layer, time_txt);
	text_layer_set_text(&date_layer, date_txt);
}

void init(AppContextRef ctx) {
	window_init(&window, "Minitime");
	window_stack_push(&window, true);

	text_layer_init(&time_layer, GRect(0, 60, 144, 24));
	text_layer_set_text_alignment(&time_layer, GTextAlignmentCenter);
	text_layer_set_font(&time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));

	text_layer_init(&date_layer, GRect(0, 84, 144, 24));
	text_layer_set_text_alignment(&date_layer, GTextAlignmentCenter);
	text_layer_set_font(&date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));

	layer_add_child(&window.layer, &time_layer.layer);
	layer_add_child(&window.layer, &date_layer.layer);

	tick();
}

void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &init,
		.tick_info = {
			.tick_handler = &tick,
			.tick_units = MINUTE_UNIT
		},
	};
	app_event_loop(params, &handlers);
}
