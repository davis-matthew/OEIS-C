/*
 * A026955  a(n) = Sum_{k=0..n} (k+1) * T(n,k), with T given by A026386.
 * https://oeis.org/A026955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2) * (7 - 3*(-1)^n) * 5^floor(n/2) / 10 (n>0),
 *            a(0)=1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* floor(n/2) */
    idx_t k = n >> 1;

    if ((n & 1) == 0)   /* even n (n≥2) */
    {
        /* a(n) = 2*(n+2)*5^(k-1) */
        return ((seq_t)(n + 2)) * ((seq_t)2) * power5(k - 1);
    }
    else                /* odd n */
    {
        /* a(n) = (n+2)*5^k */
        return ((seq_t)(n + 2)) * power5(k);
    }
}
