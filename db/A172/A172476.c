/*
 * A172476  a(n) = floor(n/sqrt(6)).
 * https://oeis.org/A172476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n / sqrt(6))
 * Range:     u8 n <= 15, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 984 (2026-11-01)
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
    seq_t n_cast = (seq_t)n;
    seq_t n2     = n_cast * n_cast;   /* n² */
    seq_t s      = n2 / 6;            /* floor(n² / 6) */
    return floor_sqrt(s);            /* floor(sqrt(s)) = floor(n / sqrt(6)) */
}
