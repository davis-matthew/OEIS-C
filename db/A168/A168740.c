/*
 * A168740  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = N(n) + 13 * Σ_{j=1}^{min(n,17)} a(n-j) – 91 * a(n-18)
 *            where N(0) = 1 N(1)…N(17) = 2 N(18) = 1 N(k) = 0 for k > 18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure space for indices 0..n */
    if ((size_t)n + 1 > cap)
    {
        if (cache == NULL)
            cache = (seq_t *)calloc((n + 1), sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

        /* Compute new terms one by one */
        for (idx_t i = cap; i <= n; ++i)
        {
            seq_t sum = 0;
            /* Sum the previous up to 17 terms */
            for (idx_t j = 1; j <= 17 && j <= i; ++j)
                sum += cache[i - j];

            seq_t val = sum * (seq_t)13;   /* 13 * Σ a(i-j) */

            /* Add N(i) */
            seq_t N = 0;
            if (i == 0 || i == 18)
                N = 1;
            else if (i >= 1 && i <= 17)
                N = 2;
            val += N;

            /* Subtract 91*a(i-18) when i ≥ 18 */
            if (i >= 18)
                val -= (seq_t)91 * cache[i - 18];

            cache[i] = val;
        }
        cap = n + 1;
    }
    return cache[n];
}
