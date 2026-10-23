/*
 * A030050  Numbers from the Conway-Schneeberger 15-theorem.
 * https://oeis.org/A030050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   a(2n-1) = n*(n+1)/2 a(2n) = Sum_{d | (n+1)} mu(d) * ( (n+1)/d *
 *            ((n+1)/d + 1) / 2 ) mu(d) is the Möbius function. This
 *            definition yields the infinite sequence of the numbers appearing
 *            in the Conway–Schneeberger 15‑theorem
 *            (1,2,3,5,6,7,10,13,15,27,…)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th triangular number, using seq_t arithmetic. */
static inline seq_t triangular(idx_t n)
{
    return (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;
}

/* Compute the Möbius function of n.  n is assumed >0. */
static inline int mobius(idx_t n)
{
    int mu = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            temp /= p;
            if (temp % p == 0)   /* square factor, mu = 0 */
                return 0;

            mu = -mu;           /* one distinct prime factor */
            while (temp % p == 0) /* skip repeated powers (already handled) */
                temp /= p;
        }
    }

    if (temp > 1)           /* remaining prime factor */
        mu = -mu;

    return mu;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n<1, return 0 as a safety */

    if (n & 1) /* odd index: a(2k-1) */
    {
        idx_t k = (n + 1) / 2;
        return triangular(k);
    }
    else /* even index: a(2k) */
    {
        idx_t k   = n / 2;
        idx_t m   = k + 1;         /* m = k+1 as in the definition */
        seq_t sum = 0;

        for (idx_t d = 1; d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                int mu_d = mobius(d);
                if (mu_d != 0)
                    sum += (seq_t)mu_d * triangular(m / d);

                if (d != m / d)
                {
                    int mu_d2 = mobius(m / d);
                    if (mu_d2 != 0)
                        sum += (seq_t)mu_d2 * triangular(d);
                }
            }
        }
        return sum;
    }
}
