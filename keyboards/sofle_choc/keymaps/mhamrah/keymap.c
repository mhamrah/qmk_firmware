/* Copyright 2023 Brian Low
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_matrix_user.h"

#ifdef OLED_ENABLE
#    include "crab.c"
#endif

char rgb_str[10];

// Custom keycodes and aliases
enum custom_keycodes {
    RGB_NEXT = SAFE_RANGE,
};

// Short aliases for RGB Matrix keycodes (for visual alignment in keymap)
#define RGB_PREV QK_RGB_MATRIX_MODE_PREVIOUS
#define RGB_HUI QK_RGB_MATRIX_HUE_UP
#define RGB_HUD QK_RGB_MATRIX_HUE_DOWN
#define RGB_SAI QK_RGB_MATRIX_SATURATION_UP
#define RGB_SAD QK_RGB_MATRIX_SATURATION_DOWN
#define RGB_VAI QK_RGB_MATRIX_VALUE_UP
#define RGB_VAD QK_RGB_MATRIX_VALUE_DOWN
#define RGB_SPI QK_RGB_MATRIX_SPEED_UP
#define RGB_SPD QK_RGB_MATRIX_SPEED_DOWN

// Get RGB mode name using QMK's RGB_MATRIX_* constants so indices remain correct
const char *get_rgb_mode_name(uint8_t mode) {
    switch (mode) {
        case RGB_MATRIX_NONE:
            return "None";
        case RGB_MATRIX_SOLID_COLOR:
            return "Static";
        case RGB_MATRIX_CYCLE_UP_DOWN:
            return "CycleUpDn";
        case RGB_MATRIX_CYCLE_OUT_IN:
            return "CycleOutIn";
        case RGB_MATRIX_CYCLE_PINWHEEL:
            return "Pinwheel";
        case RGB_MATRIX_DUAL_BEACON:
            return "DualBeam";
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            return "RainbowPin";
        case RGB_MATRIX_HUE_BREATHING:
            return "HueBreathe";
        default:
            sprintf(rgb_str, "M%d", mode);
            return rgb_str;
    }
}

// Custom 24x24 layer bitmaps (each array is 72 bytes for 24x24 pixels)
static const uint8_t PROGMEM layer_main_bitmap[] = {
    // Main layer - Clean home/house icon - 24x24
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x00, 0xFF, 0x00, 0x01, 0xFF, 0x80, 0x03, 0xFF, 0xC0, 0x07, 0xFF, 0xE0, 0x0F, 0xFF, 0xF0, 0x1F, 0xFF, 0xF8, 0x3F, 0xFF, 0xFC, 0x7F, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};

static const uint8_t PROGMEM layer_symbols_bitmap[] = {
    // Symbols layer - Brackets and symbols - 24x24
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x78, 0x3F, 0x00, 0xFC, 0x3F, 0x00, 0xFC, 0x38, 0x00, 0x1C, 0x38, 0x00, 0x1C, 0x38, 0x00, 0x1C, 0x38, 0x7E, 0x1C, 0x38, 0xFF, 0x1C, 0x39, 0xFF, 0x9C, 0x3B, 0xFF, 0xDC, 0x3B, 0xFF, 0xDC, 0x39, 0xFF, 0x9C, 0x38, 0xFF, 0x1C, 0x38, 0x7E, 0x1C, 0x38, 0x00, 0x1C, 0x38, 0x00, 0x1C, 0x38, 0x00, 0x1C, 0x3F, 0x00, 0xFC, 0x3F, 0x00, 0xFC, 0x1E, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t PROGMEM layer_util_bitmap[] = {
    // Util layer - Clean gear/settings icon - 24x24
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x01, 0xFF, 0x80, 0x03, 0xFF, 0xC0, 0x07, 0xE7, 0xE0, 0x0F, 0xC3, 0xF0, 0x1F, 0x81, 0xF8, 0x3F, 0x00, 0xFC, 0x7E, 0x00, 0x7E, 0xFC, 0x00, 0x3F, 0xFC, 0x00, 0x3F, 0x7E, 0x00, 0x7E, 0x3F, 0x00, 0xFC, 0x1F, 0x81, 0xF8, 0x0F, 0xC3, 0xF0, 0x07, 0xE7, 0xE0, 0x03, 0xFF, 0xC0, 0x01, 0xFF, 0x80, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00};

static const uint8_t PROGMEM radar_frame4[] = {
    // Frame 4: Sweep at top left
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

enum layer_names { _MAC_DEFAULT, _MAC_SYMBOLS, _MAC_UTIL };

// Function to draw 24x24 bitmap by setting individual pixels
void oled_write_bitmap_24x24(const uint8_t *bitmap, uint8_t start_col, uint8_t start_row, bool invert) {
    for (uint8_t row = 0; row < 24; row++) {
        // Each row is 3 bytes (24 bits) for 24x24 bitmap
        uint8_t byte1 = pgm_read_byte(&bitmap[row * 3]);     // High byte
        uint8_t byte2 = pgm_read_byte(&bitmap[row * 3 + 1]); // Middle byte
        uint8_t byte3 = pgm_read_byte(&bitmap[row * 3 + 2]); // Low byte

        if (invert) {
            byte1 = ~byte1;
            byte2 = ~byte2;
            byte3 = ~byte3;
        }

        // Draw first 8 pixels from high byte
        for (uint8_t col = 0; col < 8; col++) {
            bool pixel_on = (byte1 & (1 << (7 - col))) != 0;
            oled_write_pixel(start_col + col, start_row + row, pixel_on);
        }

        // Draw next 8 pixels from middle byte
        for (uint8_t col = 0; col < 8; col++) {
            bool pixel_on = (byte2 & (1 << (7 - col))) != 0;
            oled_write_pixel(start_col + 8 + col, start_row + row, pixel_on);
        }

        // Draw last 8 pixels from low byte
        for (uint8_t col = 0; col < 8; col++) {
            bool pixel_on = (byte3 & (1 << (7 - col))) != 0;
            oled_write_pixel(start_col + 16 + col, start_row + row, pixel_on);
        }
    }
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_MAC_DEFAULT] = LAYOUT(
    KC_GRV,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                                          KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINUS,
    KC_TAB,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                          KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_EQUAL,
    KC_RCTL, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                          KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_B,   KC_Z,   KC_X,   KC_C,   KC_V, KC_MUTE,                        KC_MPLY, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,

        RGB_NEXT, LSA_T(KC_TAB), LOPT_T(KC_ESC), MO(1), LCMD_T(KC_SPC),        KC_SPC, KC_ENT, LCTL_T(KC_TAB), MO(2), SCMD(KC_SPC)
    ),


    [_MAC_SYMBOLS] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS, KC_TRNS,                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS, KC_TRNS,                               KC_TRNS, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_PAGE_DOWN, KC_PAGE_UP, KC_TRNS, KC_TRNS,                               KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, KC_BSLS,
    KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                           KC_BSPC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),


    [_MAC_UTIL] = LAYOUT(
        RGB_NEXT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                 KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  QK_BOOT,
        RGB_NEXT, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, KC_F11,                                KC_F12,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, EE_CLR,
        RGB_PREV, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, KC_TRNS,                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_NEXT, KC_MPLY,            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PAGE_DOWN, KC_PAGE_UP)},
    {ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    {ENCODER_CCW_CW(RGB_PREV, RGB_NEXT), ENCODER_CCW_CW(RGB_HUD, RGB_HUI)},
};
#endif

// Helper to identify RGB Matrix control keycodes (new QMK API)
static bool is_rgb_matrix_keycode(uint16_t keycode) {
    switch (keycode) {
        case QK_RGB_MATRIX_ON:
        case QK_RGB_MATRIX_OFF:
        case QK_RGB_MATRIX_TOGGLE:
        case QK_RGB_MATRIX_MODE_NEXT:
        case QK_RGB_MATRIX_MODE_PREVIOUS:
        case QK_RGB_MATRIX_HUE_UP:
        case QK_RGB_MATRIX_HUE_DOWN:
        case QK_RGB_MATRIX_SATURATION_UP:
        case QK_RGB_MATRIX_SATURATION_DOWN:
        case QK_RGB_MATRIX_VALUE_UP:
        case QK_RGB_MATRIX_VALUE_DOWN:
        case QK_RGB_MATRIX_SPEED_UP:
        case QK_RGB_MATRIX_SPEED_DOWN:
            return true;
        default:
            return false;
    }
}
// Global key counter for OLED display
static uint16_t key_count = 0;

// Handle custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle RGB_NEXT custom keycode
    if (keycode == RGB_NEXT && record->event.pressed) {
        if (!rgb_matrix_is_enabled()) {
            // If RGB is off, turn it on and set to first mode
            rgb_matrix_enable();
            rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
        } else {
            uint8_t current_mode = rgb_matrix_get_mode();
            // Cycle through specific modes, then turn off
            switch (current_mode) {
                case RGB_MATRIX_SOLID_COLOR:
                    rgb_matrix_mode(RGB_MATRIX_CYCLE_UP_DOWN);
                    break;
                case RGB_MATRIX_CYCLE_UP_DOWN:
                    rgb_matrix_mode(RGB_MATRIX_CYCLE_OUT_IN);
                    break;
                case RGB_MATRIX_CYCLE_OUT_IN:
                    rgb_matrix_mode(RGB_MATRIX_CYCLE_PINWHEEL);
                    break;
                case RGB_MATRIX_CYCLE_PINWHEEL:
                    rgb_matrix_mode(RGB_MATRIX_DUAL_BEACON);
                    break;
                case RGB_MATRIX_DUAL_BEACON:
                    rgb_matrix_mode(RGB_MATRIX_RAINBOW_PINWHEELS);
                    break;
                case RGB_MATRIX_RAINBOW_PINWHEELS:
                    rgb_matrix_mode(RGB_MATRIX_HUE_BREATHING);
                    break;
                case RGB_MATRIX_HUE_BREATHING:
                    // Turn off RGB after last mode
                    rgb_matrix_disable();
                    break;
                default:
                    // If on an unlisted mode, go to first mode
                    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
                    break;
            }
        }
        return false; // Skip further processing
    }

    // Increment key counter for all keys except RGB control keys
    if (record->event.pressed) {
        // Don't count RGB control keys for key counter
        if (!is_rgb_matrix_keycode(keycode) && keycode != RGB_NEXT) {
            key_count++;
        }
    }
    return true; // Allow QMK to process all keycodes normally
}

// OLED
#ifdef OLED_ENABLE

// Rotate OLED
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // Make slave vertical like master
    }
    return OLED_ROTATION_270;
}

// Draw to OLED
bool oled_task_user() {
    if (is_keyboard_master()) {
        // Clear the display first to prevent random pixels
        oled_clear();

        // Master side - Layer icon at top, WPM underneath
        uint8_t current_layer = get_highest_layer(layer_state);

        // Draw 24x24 layer bitmap centered horizontally at top
        // OLED is 32 pixels wide, so (32-24)/2 = 4 pixel offset for centering
        switch (current_layer) {
            case _MAC_DEFAULT:
                oled_write_bitmap_24x24(layer_main_bitmap, 4, 0, false);
                break;
            case _MAC_SYMBOLS:
                oled_write_bitmap_24x24(layer_symbols_bitmap, 4, 0, false);
                break;
            case _MAC_UTIL:
                oled_write_bitmap_24x24(layer_util_bitmap, 4, 0, false);
                break;
        }

        // Line after icon
        oled_set_cursor(0, 3);
        oled_write_P(PSTR("-----"), false);

        // Layer name centered
        oled_set_cursor(1, 4);
        switch (current_layer) {
            case _MAC_DEFAULT:
                oled_write_P(PSTR("Main"), false);
                break;
            case _MAC_SYMBOLS:
                oled_write_P(PSTR("Syms"), false);
                break;
            case _MAC_UTIL:
                oled_write_P(PSTR("Util"), false);
                break;
        }

        // Line after layer name
        oled_set_cursor(0, 5);
        oled_write_P(PSTR("-----"), false);

        // WPM label centered
        oled_set_cursor(1, 6);
        oled_write_P(PSTR("WPM"), false);

        // WPM number centered (3 characters)
        oled_set_cursor(1, 7);
        sprintf(rgb_str, "%03d", get_current_wpm());
        oled_write(rgb_str, false);

        // Line after WPM
        oled_set_cursor(0, 8);
        oled_write_P(PSTR("-----"), false);

        // RGB info with name
        oled_set_cursor(0, 9);
        if (!(rgb_matrix_is_enabled())) {
            oled_write_P(PSTR("RGB:Off"), false);
        } else {
            uint8_t     current_mode = rgb_matrix_get_mode();
            const char *mode_name    = get_rgb_mode_name(current_mode);
            oled_write(mode_name, false);
        }
    } else {
        oled_render_anim();
    }
    return false;
}

#endif
