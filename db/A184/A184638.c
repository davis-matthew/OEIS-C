/*
 * A184638  floor(nr+h), where r=sqrt(3), h=-1/2; complement of A184653.
 * https://oeis.org/A184638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3) - 1/2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Validated: partial, pass, 116 terms through n = 116 (2026-11-01)
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
    /* t = floor(n*sqrt(3)) = floor(sqrt(3*n*n)) */
    seq_t nn = (seq_t)n;
    seq_t v = (seq_t)3 * nn * nn;
    seq_t t = floor_sqrt(v);

    /* Determine whether the fractional part of n*sqrt(3) is < 0.5
       by comparing 12*n^2 with (2*t+1)^2. */
    seq_t left  = (seq_t)12 * nn * nn;
    seq_t two_t_plus1 = (seq_t)2 * t + 1;
    seq_t right = two_t_plus1 * two_t_plus1;

    if (left < right)
        return t - 1;
    else
        return t;
}
