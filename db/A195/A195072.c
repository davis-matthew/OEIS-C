/*
 * A195072  a(n) = n - floor(n/sqrt(3)).
 * https://oeis.org/A195072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n / sqrt(3))
 * Range:     u8 n <= 13, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8561 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n / sqrt(3)) as floor(sqrt(floor(n^2 / 3))) using integer arithmetic. */
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
    /* n is >= 1 for the offset, but we handle n==0 gracefully. */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                 /* n^2 */
    seq_t div3 = sq / (seq_t)3;          /* floor(n^2 / 3)   */
    seq_t k = floor_sqrt(div3);         /* floor(sqrt( floor(n^2/3) )) = floor(n / sqrt(3)) */
    return sn - k;                      /* a(n) = n - floor(n / sqrt(3)) */
}
