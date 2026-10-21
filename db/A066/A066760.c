/*
 * A066760  a(n) = Sum_{1<=k<=n, k is not a divisor of n and k is not coprime
 *          to n} k.
 * https://oeis.org/A066760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)/2 - sigma(n) - n*phi(n)/2 where sigma(n) is
 *            the sum of all divisors of n and phi(n) is Euler's totient
 *            function
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_of.h"

/* --- local helper to compute sigma(n) --------------------------- */
static seq_t sigma_of( idx_t n )
{
    seq_t sum = 0;
    for ( idx_t i = 1; i <= n / i; ++i )
    {
        if ( n % i == 0 )
        {
            sum += (seq_t)i;
            if ( i != n / i )
                sum += (seq_t)(n / i);
        }
    }
    return sum;
}

/* --- main sequence function ----------------------------------- */
seq_t a( idx_t n )
{
    if ( n <= 1 )
        return (seq_t)0;                 /* a(1) = 0 by definition */

    seq_t nseq   = (seq_t)n;
    seq_t half   = nseq * (nseq + 1) / 2;   /* n*(n+1)/2 */
    seq_t sigma  = sigma_of( n );            /* sum of divisors of n */
    seq_t phi    = phi_of( n );              /* Euler's totient of n */
    seq_t nphi   = nseq * phi / 2;           /* n*phi(n)/2 */

    return 1 + half - sigma - nphi;
}
