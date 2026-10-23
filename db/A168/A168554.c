/*
 * A168554  G.f.: A(x) = Sum_{n>=0} 2^(n^2)*A000108(n)*(1-2^n*x)^n*x^n where
 *          A000108 is the Catalan numbers.
 * https://oeis.org/A168554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-1)^k * 2^(n(n-k)) * binom(n-k,k) *
 *            catalan(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binom_n_k, catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom_n_k.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    idx_t maxk = n / 2;                 /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* exponent for 2^(n(n-k)) */
        idx_t exp = (idx_t)((seq_t)n * ((seq_t)n - (seq_t)k));

        seq_t term = pow2(exp);                     /* 2^(n(n-k)) */
        term *= binom_n_k(n - k, k);                /* * binom(n-k,k) */
        term *= catalan(n - k);                     /* * catalan(n-k) */

        if (k & 1U)                                 /* odd k -> subtract */
            neg += term;
        else                                         /* even k -> add   */
            pos += term;
    }

    return pos - neg;          /* pos ≥ neg always for this sequence */
}
