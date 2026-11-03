/*
 * A195119  a(n) = 2*n - floor(n*sqrt(2)).
 * https://oeis.org/A195119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - floor(n*sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9560 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* floor(n*sqrt(2)) equals floor_sqrt(2*n*n) */

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
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)2 * nn * nn;   /* 2*n^2 */
    seq_t r = floor_sqrt(prod);        /* floor(n*sqrt(2)) */
    return (seq_t)2 * nn - r;          /* 2*n - floor(n*sqrt(2)) */
}
