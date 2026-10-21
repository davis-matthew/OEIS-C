/*
 * A093813  Number of letters in the names of U.S. states, alphabetically.
 * https://oeis.org/A093813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of letters in the name of the n‑th U.S. state *
 *            when the states are listed alphabetically. Letters are * counted
 *            case‑insensitively; spaces are ignored. If n is * outside the
 *            range [1,50] the function returns 0
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   count_letters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <ctype.h>
#include "../utils/count_letters.h"

static const char *state_names[50] = {
    "Alabama",         // 7
    "Alaska",          // 6
    "Arizona",         // 7
    "Arkansas",        // 8
    "California",      // 10
    "Colorado",        // 8
    "Connecticut",     // 11
    "Delaware",        // 8
    "Florida",         // 7
    "Georgia",         // 7
    "Hawaii",          // 6
    "Idaho",           // 5
    "Illinois",        // 8
    "Indiana",         // 7
    "Iowa",            // 4
    "Kansas",          // 6
    "Kentucky",        // 8
    "Louisiana",       // 9
    "Maine",           // 5
    "Maryland",        // 8
    "Massachusetts",   // 13
    "Michigan",        // 8
    "Minnesota",       // 9
    "Mississippi",     // 11
    "Missouri",        // 8
    "Montana",         // 7
    "Nebraska",        // 7
    "Nevada",          // 6
    "New Hampshire",   // 13
    "New Jersey",      // 9
    "New Mexico",      // 10
    "New York",        // 7
    "North Carolina",  // 13
    "North Dakota",    // 11
    "Ohio",            // 4
    "Oklahoma",        // 8
    "Oregon",          // 6
    "Pennsylvania",    // 12
    "Rhode Island",    // 10
    "South Carolina",  // 13
    "South Dakota",    // 11
    "Tennessee",       // 9
    "Texas",           // 5
    "Utah",            // 4
    "Vermont",         // 7
    "Virginia",        // 8
    "Washington",      // 10
    "West Virginia",   // 13
    "Wisconsin",       // 9
    "Wyoming"          // 6
};

seq_t a(idx_t n) {
    if (n < 1 || n > 50u) return (seq_t)0;
    /* Since n is 1‑based and our array is 0‑based, subtract 1. */
    return (seq_t)count_letters(state_names[(idx_t)n - 1u]);
}
