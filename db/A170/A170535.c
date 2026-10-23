/*
 * A170535  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the rational generating function A170535(t) =
 *            (t^46 + 2 t^45 + … + 2 t + 1) / ( 1 - 44 t - 44 t^2 - … - 44
 *            t^45 + 990 t^46 ). This yields the linear recursion: For 1 ≤ n ≤
 *            45: a(n) = 2 + 44 * Σ_{k=0}^{n-1} a(k) For n ≥ 46: a(n) = 44 *
 *            Σ_{k=1}^{45} a(n-k) - 990 * a(n-46) with a(...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;          /* array of terms a(0)…a(cache_len-1) */
    static idx_t  cache_len = 0;         /* number of terms stored so far */
    static size_t cache_cap = 0;         /* allocated capacity of cache */
    /* auxiliary variables for the recurrence */
    static seq_t cum_sum   = 0;   /* Σ_{k=0}^{i-1} a(k)  (used for i ≤ 45) */
    static seq_t sum_last45 = 0;  /* Σ_{k=i-45}^{i-1} a(k)  (used for i ≥ 46) */

    /* enlarge the cache if necessary */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    else if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise the first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        cum_sum  = cache[0];   /* Σ_{k=0}^{0} a(k) = a(0) */
    }

    /* if the requested value is already known, just return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 45) {
            /* recurrence for the first 45 terms */
            cache[i] = (seq_t)2 + (seq_t)44 * cum_sum;
            cum_sum += cache[i];
        } else { /* i ≥ 46 */
            if (i == 46) {
                /* initialise the sliding window sum for a(46) */
                sum_last45 = (seq_t)0;
                for (idx_t k = 1; k <= 45; ++k)
                    sum_last45 += cache[k];
            }
            cache[i] = (seq_t)44 * sum_last45 - (seq_t)990 * cache[i - 46];
            /* update the sliding window for the next step */
            sum_last45 += cache[i] - cache[i - 45];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
