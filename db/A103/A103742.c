/*
 * A103742  Number of letters in Turkish names for days of week.
 * https://oeis.org/A103742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   utf8_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of letters in Turkish names for days of week.
   a(n) = number of Unicode codepoints of day_names[(n-1) mod 7]. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/utf8_len.h"

static const char *day_names[7] = {
    "Pazartesi",
    "Salı",
    "Çarşamba",
    "Perşembe",
    "Cuma",
    "Cumartesi",
    "Pazar"
};

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* index below offset, any value ok */
    idx_t idx = (n - 1) % 7;
    return utf8_len(day_names[(int)idx]);
}
