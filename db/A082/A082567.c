/*
 * A082567  Palindromic time display in hours, minutes, seconds on a six
 *          spaced 24-hour digital clock, using hours 1-24.
 * https://oeis.org/A082567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 355, u32 n <= 606, u64 n <= 606, u128 n <= 606,
 *            big any n
 * Helpers:   rev_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic time sequence A082567 (offset 1):
   a(n) is the decimal value of the nth palindromic clock reading
   (hours 1–24, minutes 00–59, seconds 00–59).  For hours 1–9 the
   reading has 5 digits: [h][m_tens][m_ones][m_tens][h]; for
   hours 10–24 it has 6 digits: [ht][ho][m_tens][m_ones][ho][ht].
   The table is built once and returned in O(1). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev_two.h"

#define PAL_COUNT 606

static seq_t pal_values[PAL_COUNT];
static bool pal_inited = false;

static void init_pal_values(void)
{
    if (pal_inited)
        return;

    size_t idx = 0;

    /* Hours 1–9: 5‑digit palindromes. */
    for (uint8_t h = 1; h <= 9; ++h) {
        for (uint8_t m = 0; m <= 59; ++m) {
            /* Seconds are formed by the minute‑tens digit followed by the hour. */
            seq_t sec = (seq_t)(m / 10) * 10 + (seq_t)h;
            seq_t val = (seq_t)h * 10000
                      + (seq_t)m * 100
                      + sec;
            pal_values[idx++] = val;
        }
    }

    /* Hours 10–24: 6‑digit palindromes. */
    for (uint8_t h = 10; h <= 24; ++h) {
        seq_t rev_h = rev_two(h);          /* seconds must be rev(hour). */
        if (rev_h > 59)                    /* discard if >59. */
            continue;
        for (uint8_t x = 0; x <= 5; ++x) { /* minute = 00,11,22,33,44,55. */
            seq_t minute = x * 11;
            seq_t val = (seq_t)h * 10000
                      + (seq_t)minute * 100
                      + rev_h;
            pal_values[idx++] = val;
        }
    }

    /* idx must equal PAL_COUNT (606). */
    pal_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > PAL_COUNT)
        return (seq_t)0;
    init_pal_values();
    return pal_values[(size_t)n - 1];
}
