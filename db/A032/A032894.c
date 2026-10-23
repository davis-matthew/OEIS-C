/*
 * A032894  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   zigzag6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032894: Numbers whose base‑6 digits satisfy d_0 ≥ d_1 ≤ d_2 ≥ d_3 ≤ …
   This implementation scans the natural numbers and counts those that
   satisfy the alternating pattern, returning the n‑th such number.  It
   works for any seq_t type that supports +, ++, %, and / by a small
   integer.  No caching is used. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/zigzag6.h"

seq_t a(idx_t n)
{
    seq_t val = 1;
    idx_t count = 0;

    while (1) {
        if (zigzag6(val)) {
            ++count;
            if (count == n) return val;
        }
        val = val + 1;   /* works for both integral and big‑int types */
    }
}
