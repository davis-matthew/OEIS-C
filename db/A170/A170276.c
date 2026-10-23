/*
 * A170276  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤40, a(n)=27·26^(n‑1); for n≥41, a(n) =
 *            (25·∑_{k=n-40}^{n-1} a(k) + a(n-41)) / 325
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed values   */
    static idx_t cache_size = 0;
    static idx_t max_computed = (idx_t)-1; /* last index that has been computed */

    /* initialise on first call */
    if (max_computed == (idx_t)-1)
    {
        cache_size   = (n >= 1 ? n + 1 : 1);
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0]     = seq_one();          /* a(0) = 1 */
        max_computed = 0;
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_size)
    {
        cache_size = n + 1;
        cache      = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* value already known */
    if (n <= max_computed)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i <= 40)                 /* initial block 1 ≤ i ≤ 40 */
        {
            if (i == 1)
                cache[i] = (seq_t)27;
            else
                cache[i] = cache[i - 1] * (seq_t)26;
        }
        else                          /* recurrence for i ≥ 41 */
        {
            /* maintain a sliding window sum of the last 40 terms */
            static seq_t sum_last40 = 0;
            /* initialise sum when first entering the recurrence */
            if (i == 41)
            {
                sum_last40 = 0;
                for (idx_t j = 1; j <= 40; ++j)
                    sum_last40 += cache[j];
            }
            else
            {
                /* update the window: add current term (not yet computed) */
                /* will be updated after computing cache[i] below */
            }

            /* compute a(i) using the recurrence */
            cache[i] =
                ((seq_t)25 * sum_last40 + cache[i - 41]) / (seq_t)325;

            /* update sliding window for the next step */
            sum_last40 += cache[i];
            if (i >= 40)
                sum_last40 -= cache[i - 40];
        }
    }

    max_computed = n;
    return cache[n];
}
