/*
 * A066096  a(n) = floor(n*phi), where phi = (1 + sqrt(5))/2.
 * https://oeis.org/A066096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi) = floor((n + sqrt(5*n*n))/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9483 (2026-11-01)
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
    seq_t nn = (seq_t)n;
    /* t = 5 * n^2  */
    seq_t t = (seq_t)5 * nn * nn;
    /* i = floor(sqrt(t)) = floor(n*sqrt(5)) */
    seq_t i = floor_sqrt(t);
    /* a(n) = floor((n + i)/2) */
    return (nn + i) / 2;
}
