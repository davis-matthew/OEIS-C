/*
 * A189363  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(2), t=sqrt(3).
 * https://oeis.org/A189363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√3) + floor(n·√(2/3)) * Computed exactly
 *            using integer arithmetic: * a(n) = n + floor_sqrt(n²/3) +
 *            floor_sqrt(2·n²/3)
 * Range:     u8 n <= 9, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 100 terms through n = 928 (2026-11-01)
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
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Squared value (safe for 10⁴; overflow only an issue for very large n) */
    seq_t sq = sn * sn;

    /* floor_sqrt( n² / 3 ) */
    seq_t term1 = floor_sqrt( sq / 3 );

    /* floor_sqrt( 2·n² / 3 )  */
    seq_t term2 = floor_sqrt( (sq + sq) / 3 );

    return sn + term1 + term2;
}
