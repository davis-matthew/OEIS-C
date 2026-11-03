/*
 * A040674  Continued fraction for sqrt(701).
 * https://oeis.org/A040674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=floor(sqrt(701)); for n>0, a(n) are the terms of the
 *            continued fraction expansion of sqrt(701)
 * Range:     u8 none, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
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
    const seq_t S  = (seq_t)701;
    const seq_t a0 = floor_sqrt(S);           /* 26 */

    if (n == 0)
        return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    for (idx_t i = 0; i < n; ++i) {
        m = d * a - m;
        d = (S - m * m) / d;
        a = (a0 + m) / d;
    }
    return a;
}
