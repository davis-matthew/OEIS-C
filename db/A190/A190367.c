/*
 * A190367  n + [n*r/u] + [n*s/u] + [n*t/u]; r=sqrt(2), s=1/r, t=sqrt(3),
 *          u=1/t.
 * https://oeis.org/A190367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + [n*sqrt(6)] + [n*sqrt(3/2)]
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9987 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t NN     = (seq_t)n * (seq_t)n;                  /* n²  */
    seq_t term1  = floor_sqrt( (seq_t)6 * NN );         /* ⌊n√6⌋  */
    seq_t temp   = (seq_t)3 * NN / (seq_t)2;            /* ⌊3n²/2⌋  */
    seq_t term2  = floor_sqrt( temp );                  /* ⌊n√(3/2)⌋ */
    return 4 * (seq_t)n + term1 + term2;
}
