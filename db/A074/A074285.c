/*
 * A074285  Sum of the divisors of n-th triangular number.
 * https://oeis.org/A074285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n · (n+1)/2) (sum of divisors of the n‑th triangular
 *            number)
 * Range:     u8 n <= 14, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: compute the sum of divisors σ(m) for a 64‑bit (or larger) integer m.
   The routine uses trial division up to √m.  This is fast enough for n ≤ 10 000
   and works for any seq_t size supported by the build. */
static seq_t sigma_of(seq_t m)
{
    seq_t sigma = 1;
    seq_t rem = m;
    for (seq_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            /* p is a prime factor; count its exponent e and the power p^e */
            seq_t p_pow = 1;      /* p^e */
            unsigned int e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
                p_pow *= p;
            }
            /* compute (p^(e+1) − 1) / (p − 1) and multiply into sigma */
            seq_t numerator = p * p_pow - 1;   /* p^(e+1) - 1 */
            seq_t denom     = p - 1;
            sigma *= numerator / denom;
        }
    }
    if (rem > 1)   /* a remaining prime factor of exponent 1 */
        sigma *= (1 + rem);
    return sigma;
}

/* Main sequence function a(n) as required by the assignment */
seq_t a(idx_t n)
{
    /* Compute the n-th triangular number T_n = n·(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Return σ(T_n) */
    return sigma_of(tn);
}
