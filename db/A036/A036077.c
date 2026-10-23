/*
 * A036077  The number of partitions of {1..7n} that are invariant under a
 *          permutation consisting of n 7-cycles.
 * https://oeis.org/A036077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of {1..7n} that are invariant under
 *            a permutation consisting of n 7‑cycles. Recurrence: a(0)=1;
 *            a(n)=∑_{k=1}^{n} C(n-1,k-1)*(1+7^{k-1})*a(n-k) (n≥1) Initial
 *            values: a(0)=1, a(1)=2, a(2)=12, a(3)=106, a(4)=1144, …
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Dynamic cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t idx = (size_t)n;
    if (idx + 1 > cache_size)
    {
        /* grow the cache */
        size_t new_size = idx + 1;
        if (cache == NULL)
        {
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
            cache[0] = 1;          /* a(0) = 1 */
            cache_size = 1;
        }
        else
        {
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }

        /* compute missing terms */
        for (size_t m = cache_size; m <= idx; ++m)
        {
            seq_t sum = 0;
            seq_t binom = 1;    /* C(m-1,0) */
            seq_t pow7 = 1;     /* 7^{k-1} for current k */
            for (size_t k = 1; k <= m; ++k)
            {
                seq_t ck = pow7 + 1;                     /* c_k = 1 + 7^{k-1} */
                seq_t term = binom * ck;
                term = term * cache[m - k];
                sum += term;

                if (k < m)
                {
                    /* binom = C(m-1,k) from C(m-1,k-1) */
                    binom = binom * (seq_t)(m - k);
                    binom = binom / (seq_t)k;
                }
                pow7 *= 7;    /* prepare 7^{k} for next k+1 */
            }
            cache[m] = sum;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
