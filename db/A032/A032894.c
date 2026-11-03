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
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
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

/* Return true if x's base‑6 representation alternates:
   d0 ≥ d1 ≤ d2 ≥ d3 …   (d0 is the least significant digit) */
static bool zigzag6(seq_t x)
{
    if (x <= 5) return true;                /* single digit is always ok */

    uint8_t prev = (uint8_t)(x % 6);
    x = x / 6;
    size_t i = 1;                            /* index of current digit */

    while (x > 0) {
        uint8_t cur = (uint8_t)(x % 6);
        if ((i & 1) == 1) {                 /* odd i: prev ≥ cur */
            if (prev < cur) return false;
        } else {                             /* even i: prev ≤ cur */
            if (prev > cur) return false;
        }
        prev = cur;
        x = x / 6;
        ++i;
    }
    return true;
}

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
