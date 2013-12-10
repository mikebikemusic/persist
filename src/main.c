#include <pebble.h>

#define AKEY_TIMEZONE 1
#define AKEY_TZCITY 10
#define PEBBLE_SCREEN_WIDTH 144

static Window *window;
static TextLayer *text_layer;

static char initialCity[40];
static char buffer[80];
static int initialselected;

static void handle_tick(struct tm* now, TimeUnits units_changed)
{
	 char tzCity[40];
	 char newCity[40];

	int selected = now->tm_min;
    snprintf(tzCity, sizeof(tzCity), "city %d", selected);

  snprintf(buffer, sizeof(buffer), " init string: %s\n city string: %s\n init int: %d\n new int: %d", initialCity, tzCity, initialselected, selected);

		persist_write_int(AKEY_TIMEZONE, selected);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "new int value = %d", selected);
		int reselected = persist_read_int(AKEY_TIMEZONE);
		APP_LOG(APP_LOG_LEVEL_DEBUG,"new int value reads back as %d", reselected);

		persist_write_string(AKEY_TZCITY, tzCity);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "new city = %s", tzCity);
  		strcpy(newCity, "unchanged");
		if ( E_DOES_NOT_EXIST == persist_read_string(AKEY_TZCITY, newCity, sizeof(newCity)))
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Key %d does not exist", AKEY_TZCITY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "new city read back as = %s", newCity);

  text_layer = text_layer_create((GRect) { .origin = { 0, 50 }, .size = { PEBBLE_SCREEN_WIDTH, 100 } });
  text_layer_set_text(text_layer, buffer);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

}

static void init(void) {
  window = window_create();
  const bool animated = true;
  window_stack_push(window, animated);
	 char tzCity[40];
  	strcpy(tzCity, "waiting...");
	if ( E_DOES_NOT_EXIST == persist_read_string(AKEY_TZCITY, tzCity, sizeof(tzCity)))
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Key %d does not exist", AKEY_TZCITY);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "initial city = %s", tzCity);
	strcpy(initialCity, tzCity);

	time_t selected = persist_read_int(AKEY_TIMEZONE);
	initialselected = selected;
	APP_LOG(APP_LOG_LEVEL_DEBUG,"initial int value %ld", selected);
	
	tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);
}

static void deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
}
