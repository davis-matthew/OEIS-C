/*
 * A128152  Numerator of Sum_{k=0..n} 1/binomial(n,k)^4.
 * https://oeis.org/A128152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator( Sum_{k=0..n} 1 / binomial(n,k)^4 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the numerator of the sum of reciprocals of fourth powers of
   binomial coefficients for a given n.  The denominator of the sum
   is fully cancelled, so only the reduced numerator is returned. */
seq_t a(idx_t n)
{
    seq_t num = 0;      /* current numerator of the sum   */
    seq_t den = 1;      /* current denominator of the sum */

    seq_t binom = 1;    /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* denominator of the current term: binom^4 */
        seq_t d = binom * binom;      /* binom^2 */
        d = d * d;                    /* binom^4 */

        /* Add the term 1/d to the running fraction num/den */
        seq_t g   = gcd(den, d);          /* gcd(den,d) */
        seq_t m1  = d / g;                /* d/g   */
        seq_t m2  = den / g;              /* den/g*/

        /* new numerator = num * (d/g) + 1 * (den/g) */
        num = num * m1 + m2;

        /* new denominator = lcm(den,d) = den/g * d */
        den = den / g * d;

        /* reduce the fraction to keep numbers smaller */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* update binom for next k:
           binom(n,k+1) = binom(n,k) * (n-k) / (k+1) */
        if (k < n)
            binom = binom * ( (seq_t)(n - k) ) / ( (seq_t)(k + 1) );
    }

    /* ensure final fraction is in lowest terms (should already be) */
    seq_t g = gcd(num, den);
    num /= g;

    return num;
}
