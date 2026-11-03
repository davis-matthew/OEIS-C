/*
 * A189380  a(n) = n + floor(n*s/r) + floor(n*t/r); r=1, s=-1+sqrt(2),
 *          t=1+sqrt(2).
 * https://oeis.org/A189380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*floor_sqrt(2*n*n)
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9543 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = n + floor(n*(-1+sqrt(2))) + floor(n*(1+sqrt(2))) */

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
    /* Compute 2*n*n safely in seq_t */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t two_nn = (seq_t)2 * nn;

    /* floor_sqrt gives floor(sqrt(x)) */
    seq_t sq = floor_sqrt(two_nn);

    /* a(n) = n + 2*floor_sqrt(2*n*n) */
    return (seq_t)n + (seq_t)2 * sq;
}
