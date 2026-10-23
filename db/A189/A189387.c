/*
 * A189387  a(n) = n+[nr/s]+[nt/s]; r=1, s=sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/sqrt(2)) + floor(n/sqrt(6))
 * Range:     u8 n <= 11, u16 n <= 84, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed exactly using integer arithmetic:
   floor(n/sqrt(2)) = floor( sqrt( floor(n*n/2) ) )
   floor(n/sqrt(6)) = floor( sqrt( floor(n*n/6) ) )
   The helper floor_sqrt provides the required integer square root. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* floor(n / sqrt(2)) = floor( sqrt( floor(n*n/2) ) ) */
    seq_t t1 = floor_sqrt( nn / 2 );

    /* floor(n / sqrt(6)) = floor( sqrt( floor(n*n/6) ) ) */
    seq_t t2 = floor_sqrt( nn / 6 );

    return (seq_t)n + t1 + t2;
}
