/*
 * A140868  a(n) = floor(floor(n*alpha)*alpha) where alpha = 1+sqrt(2) =
 *          A014176.
 * https://oeis.org/A140868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(floor(n*alpha)*alpha) where alpha = 1+sqrt(2)
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 9121, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9969 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed with integer arithmetic, using the integer */
/* square root routine floor_sqrt from the utils library. */

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
    /* First part: floor(n*alpha) = n + floor(n*sqrt(2))          */
    /* Compute floor(n*sqrt(2)) as floor(sqrt(2*n^2))                */
    seq_t two_n_sq = (seq_t)2 * (seq_t)n * (seq_t)n;
    seq_t fnsqrt2 = floor_sqrt(two_n_sq);
    seq_t m = (seq_t)n + fnsqrt2;          /* m = floor(n*alpha) */

    /* Second part: floor(m*alpha) = m + floor(m*sqrt(2))          */
    /* Compute floor(m*sqrt(2)) as floor(sqrt(2*m^2))                */
    seq_t two_m_sq = (seq_t)2 * (seq_t)m * (seq_t)m;
    seq_t fmsqrt2 = floor_sqrt(two_m_sq);

    return m + fmsqrt2;
}
