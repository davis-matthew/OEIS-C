/*
 * A128441  a(n)=Floor(n*2^(n/2)).
 * https://oeis.org/A128441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * 2^(n/2) )
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp); */
#include "../utils/floor_sqrt.h"/* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* For safety treat n == 0 as 0; the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    if ((n & 1) == 0)            /* even n = 2k */
    {
        idx_t k = n >> 1;          /* k = n/2 */
        /* a = n * 2^k */
        return ((seq_t)n) * pow2(k);
    }
    else                          /* odd n = 2k + 1 */
    {
        idx_t k = n >> 1;          /* k = floor(n/2) */
        /* I = n * 2^k = (2k+1) * 2^k   */
        seq_t I = ((seq_t)n) * pow2(k);  

        /* Compute floor( sqrt( I^2 * 2 ) ) = floor( I * sqrt(2) ) = floor( n * 2^(n/2) ) */
        seq_t t = I * I;           /* I² */
        t = t + t;                 /* I² * 2 */
        return floor_sqrt(t);
    }
}
