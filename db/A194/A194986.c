/*
 * A194986  a(n) = 1 + floor(n/sqrt(6)).
 * https://oeis.org/A194986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n/sqrt(6))
 * Range:     u8 n <= 15, u16 n <= 426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9966 (2026-11-01)
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
    /* n == 0 is outside the OEIS offset; return 0. */
    if (n == 0) return 0;
    seq_t n2 = (seq_t)n * n;   /* n² */
    seq_t t  = n2 / 6;         /* floor(n² / 6) */
    seq_t m  = floor_sqrt(t);  /* floor(sqrt(t)) = floor(n / sqrt(6)) */
    return m + 1;
}
