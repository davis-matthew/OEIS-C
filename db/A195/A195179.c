/*
 * A195179  a(n) = 5*n - floor(2*n*sqrt(2)).
 * https://oeis.org/A195179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - floor(2*n*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8340 (2026-11-01)
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
    seq_t nn = (seq_t)n;           /* cast index to seq_t */
    seq_t prod = nn * nn;          /* n^2  */
    prod = prod * (seq_t)8;        /* 8*n^2  */
    seq_t b = floor_sqrt(prod);    /* floor(2*n*sqrt(2)) = floor(sqrt(8*n^2)) */
    return (seq_t)5 * nn - b;      /* 5*n - b  */
}
