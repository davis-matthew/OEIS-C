/*
 * A047932  a(n) = floor(3*n-sqrt(12*n-3)).
 * https://oeis.org/A047932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n - sqrt(12*n-3))
 * Range:     u8 n <= 5, u16 n <= 392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9017 (2026-11-01)
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
    /* Compute 12*n - 3 */
    seq_t t = (seq_t)n * 12 - 3;

    /* Integer square root of t */
    seq_t s = floor_sqrt(t);

    /* 3 * n */
    seq_t three_n = (seq_t)n * 3;

    /* Difference */
    seq_t v = three_n - s;

    /* If t is a perfect square, sqrt(t) == s, else v-1 */
    if (s * s == t)
        return v;
    else
        return v - 1;
}
