/*
 * A082845  Partial sums of A082844.
 * https://oeis.org/A082845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 + floor((n+1)*sqrt(2))
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the OEIS specification:
   a(n) = 1 + floor((1+sqrt(2))*n - (2 - sqrt(2)))  */

/* Include the integer square‑root routine from the utilities package */
#include "../utils/floor_sqrt.h"

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
