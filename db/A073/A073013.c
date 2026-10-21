/*
 * A073013  a(n) = (1/2) * Sum_{k=1..n} (n+k)!/(k!)^2.
 * https://oeis.org/A073013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2) * Sum_{k=1..n} (n+k)!/(k!)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined for offset < 1, harmless */
        return (seq_t)0;

    /* term = (n+1)!  -- for k = 1 */
    seq_t term = (seq_t)1;
    for (idx_t i = 2; i <= n + 1; ++i)
        term *= (seq_t)i;

    seq_t sum = term;

    /* iterate k = 2 .. n using the recurrence
       term_new = term * (n+k) / (k*k)  */
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t a_mul = term;                   /* current term  */
        seq_t b_mul = (seq_t)(n + k);         /* numerator extra factor */
        seq_t c_den = (seq_t)k;               /* first denominator factor  */
        seq_t d_den = (seq_t)k;               /* second denominator factor */

        /* cancel common factors to avoid overflow on 64-bit builds */
        seq_t t = gcd(a_mul, c_den);          /* cancel a and first k */
        a_mul /= t;
        c_den  /= t;

        t = gcd(a_mul, d_den);                /* cancel a and second k */
        a_mul /= t;
        d_den  /= t;

        t = gcd(b_mul, c_den);                /* cancel numerator and first denom */
        b_mul /= t;
        c_den  /= t;

        t = gcd(b_mul, d_den);                /* cancel numerator and second denom */
        b_mul /= t;
        d_den  /= t;                          /* now c_den * d_den == 1 */

        term = a_mul * b_mul;                 /* exact new term */
        sum  += term;
    }

    return sum / (seq_t)2;  /* division is exact for all n >= 1 */
}
