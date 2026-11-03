/*
 * A194152  Beatty sequence for 5+2*sqrt(5); complement of A194151.
 * https://oeis.org/A194152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(5+2*sqrt(5)))
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7212, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9850 (2026-11-01)
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
    /* n >= 1 (offset 1) */
    unsigned long long k = (unsigned long long)n * 2ULL;          /* k = 2n */
    unsigned long long tmp = 5ULL * k * k;                        /* tmp = 5*(2n)^2 */
    seq_t t = floor_sqrt((seq_t)tmp);                             /* t = floor(2n*sqrt(5)) */
    return (seq_t)n * 5ULL + t;                                   /* a(n) = 5*n + t */
}
