/*
 * A184587  a(n) = floor((n+1/2)*s), where s=(5+sqrt(5))/4; complement of
 *          A184586.
 * https://oeis.org/A184587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/2)*s) where s = (5+sqrt(5))/4
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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

/* Computes the nth element of OEIS sequence A184587. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    /* Work in seq_t to avoid overflow on intermediate steps. */
    seq_t seqn = (seq_t)n;          /* seqn = n  */
    seq_t m     = seqn * 2 + 1;      /* m = 2n+1  */
    seq_t five_m = m * 5;           /* five_m = 5*(2n+1)  */
    seq_t t      = five_m * m;      /* t = 5*(2n+1)^2  */

    /* floor((2n+1)*sqrt(5)) = floor(sqrt(5*(2n+1)^2)) */
    seq_t sqrt_term = floor_sqrt(t);

    /* Numerator of the formula: 5*(2n+1) + floor((2n+1)*sqrt(5)) */
    seq_t num = five_m + sqrt_term;

    /* Final result: floor(num / 8) */
    return num / 8;
}
