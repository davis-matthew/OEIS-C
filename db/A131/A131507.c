/*
 * A131507  2n+1 appears n+1 times.
 * https://oeis.org/A131507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*floor(sqrt(2n+1)+1/2) - 1
 * Range:     u8 n <= 31, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Validated: standard, pass, 94 terms through n = 6778 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* y = 2*n + 1 */
    seq_t y = ((seq_t)n << 1) + (seq_t)1;

    /* Compute floor sqrt of y */
    seq_t s   = floor_sqrt(y);

    /* Check whether we should round up */
    seq_t s2  = s * s;
    seq_t m   = s;
    if (y >= s2 + s + (seq_t)1)
        m = s + (seq_t)1;

    /* a(n) = 2*m – 1 */
    return m * (seq_t)2 - (seq_t)1;
}
