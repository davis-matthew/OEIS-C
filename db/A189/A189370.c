/*
 * A189370  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(2), t=sqrt(5).
 * https://oeis.org/A189370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9832 (2026-11-01)
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
    /* Convert the index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute floor(n*sqrt(2)) as floor(sqrt(2*n*n)) */
    seq_t prod2 = (seq_t)2 * nn * nn;
    seq_t f2 = floor_sqrt(prod2);

    /* Compute floor(n*sqrt(5)) as floor(sqrt(5*n*n)) */
    seq_t prod5 = (seq_t)5 * nn * nn;
    seq_t f5 = floor_sqrt(prod5);

    /* Return the sum */
    return nn + f2 + f5;
}
