/*
 * A212913  E.g.f. satisfies: A(x) = exp( Integral 1 + x*A(x)^2 dx ), where
 *          the constant of integration is zero.
 * https://oeis.org/A212913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1 and for n≥1 * a(n+1) = a(n) + n * c_{n-1}, *
 *            where * c_m = Σ_{k=0}^{m} binom(m,k) ⋅ b_k ⋅ a_{m-k}, * b_k =
 *            Σ_{i=0}^{k} binom(k,i) ⋅ a_i ⋅ a_{k-i}. * This follows from the
 *            differential equation * A'(x) = A(x) + x⋅A(x)^2⋅A(x) with A(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum supported index – sufficient for the test set */
    enum { MAX_N = 20000 };
    static seq_t a_cache[MAX_N+1];
    static seq_t b_cache[MAX_N+1];
    static idx_t cache_size = 0;          /* number of terms already computed */

    /* initialise the base case */
    if (cache_size == 0)
    {
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* grow the table if necessary */
    if (n + 1 > (idx_t)cache_size)
    {
        /* compute new terms from cache_size up to n */
        for (idx_t m = (idx_t)cache_size; m <= n; ++m)
        {
            if (m == 0) continue;          /* base already set */

            if (m == 1)
            {
                /* a(1) = a(0) */
                a_cache[1] = a_cache[0];

                /* b(1) = a0*a1 + a1*a0 = 2 */
                seq_t sum = 0;
                seq_t bin = (seq_t)1;
                for (idx_t i = 0; i <= 1; ++i)
                {
                    seq_t term = bin * a_cache[i] * a_cache[1 - i];
                    sum += term;
                    if (i < 1)
                        bin = (bin * (1 - i)) / (i + 1);
                }
                b_cache[1] = sum;
                continue;
            }

            /* compute c_{m-2} */
            idx_t sub = m - 2;
            seq_t c = (seq_t)0;
            if (sub < (idx_t)-1) /* always true for m≥2 */
            {
                seq_t binc = (seq_t)1;
                for (idx_t k = 0; k <= sub; ++k)
                {
                    seq_t term = binc * b_cache[k] * a_cache[sub - k];
                    c += term;
                    if (k < sub)
                        binc = (binc * (sub - k)) / (k + 1);
                }
            }

            /* a(m) = a(m-1) + (m-1) * c_{m-2} */
            a_cache[m] = a_cache[m - 1] + (seq_t)(m - 1) * c;

            /* compute b(m) = Σ_{i=0}^{m} binom(m,i) a_i a_{m-i} */
            seq_t sumb = (seq_t)0;
            seq_t binb = (seq_t)1;
            for (idx_t i = 0; i <= m; ++i)
            {
                seq_t term = binb * a_cache[i] * a_cache[m - i];
                sumb += term;
                if (i < m)
                    binb = (binb * (m - i)) / (i + 1);
            }
            b_cache[m] = sumb;
        }
        cache_size = (idx_t)(n + 1);
    }

    return a_cache[n];
}
