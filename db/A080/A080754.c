/*
 * A080754  a(n) = ceiling(n*(1+sqrt(2))).
 * https://oeis.org/A080754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(1+sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 76 terms through n = 4965 (2026-11-01)
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
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn;            /* n² */
    seq_t t2 = t * (seq_t)2;       /* 2·n² = (n·sqrt2)² */
    seq_t s  = floor_sqrt(t2);     /* floor(n·sqrt2) */
    return nn + s + (seq_t)1;      /* ceil(n*(1+sqrt2)) = n + floor(n·sqrt2) + 1 */
}
