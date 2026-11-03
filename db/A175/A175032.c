/*
 * A175032  a(n) is the difference between the n-th triangular number and the
 *          next perfect square.
 * https://oeis.org/A175032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = ceil(sqrt(n*(n+1)/2))^2 - n*(n+1)/2
 * Range:     u8 n <= 10, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 8605 (2026-11-01)
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
    /* compute the n-th triangular number */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* floor of the square root of T */
    seq_t s = floor_sqrt(T);

    /* if s*s < T, increase s to obtain the ceiling */
    if (s * s < T)
        ++s;

    return s * s - T;
}
