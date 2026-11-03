/*
 * A198263  a(n) = ceiling(n*sqrt(8)).
 * https://oeis.org/A198263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*sqrt(8))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8273 (2026-11-01)
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
    if (n == 0)
        return 0;

    /* m = 2*n */
    seq_t m = (seq_t)n * 2;

    /* Compute t = 2*m*m */
    seq_t t = m * m;
    t <<= 1;          /* t = 2 * (m*m) */

    /* floor_sqrt(t) gives floor(m*sqrt(2)) = floor(2*n*sqrt(2)) */
    seq_t s = floor_sqrt(t);

    return s + 1;
}
