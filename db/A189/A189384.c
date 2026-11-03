/*
 * A189384  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=1/sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(2/3))
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 997 (2026-11-01)
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
    /* Compute n^2 using the wider idx_t type to avoid overflow. */
    idx_t n_sq = n * n;
    /* temp = 2*n^2 ; both powers are done in idx_t. */
    idx_t temp = (idx_t)2 * n_sq;

    /* floor(n*sqrt(2))  = floor(sqrt(2*n^2))    */
    seq_t term1 = floor_sqrt((seq_t)temp);

    /* floor(n*sqrt(2/3))= floor(sqrt((2*n^2)/3))*/
    seq_t term2 = floor_sqrt((seq_t)(temp / (idx_t)3));

    return (seq_t)n + term1 + term2;
}
