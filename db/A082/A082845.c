/*
 * A082845  Partial sums of A082844.
 * https://oeis.org/A082845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 + floor((n+1)*sqrt(2))
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8882 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the OEIS specification:
   a(n) = 1 + floor((1+sqrt(2))*n - (2 - sqrt(2)))  */

/* Include the integer square‑root routine from the utilities package */
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

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Work in 128–bit arithmetic to avoid overflow for large indices */
    __uint128_t np1 = ( __uint128_t ) n + 1;          /* n + 1        */
    __uint128_t sq   = np1 * np1;                    /* (n+1)^2      */
    __uint128_t val  = sq << 1;                      /* 2*(n+1)^2    */

    /* floor((n+1)*sqrt(2)) = floor(sqrt(2*(n+1)^2)) */
    seq_t floor_term = floor_sqrt( (seq_t)val );

    /* a(n) = (n-1) + floor_term */
    return (seq_t)( ( ( __uint128_t ) n - 1 ) + floor_term );
}
