/*
 * A165882  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1)+18*a(n-2)+…+18*a(n-9)-171*a(n-10)+p_n * where
 *            p_n = 1 (n=0), 2 (1≤n≤9), 1 (n=10), 0 otherwise. * This
 *            recurrence follows from the rational generating function of *
 *            A165882. The sequence is computed by memoization with a dynamic
 *            * array, allowing arbitrary order of queries
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache   = NULL;   /* array of a(i) for 0 ≤ i < cache_size */
    static idx_t  cache_size = 0;   /* number of terms currently computed   */
    static size_t cache_cap  = 0;   /* allocated capacity of cache (in elems) */

    /* initialise cache if needed */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* expand cache to cover the requested index */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute missing terms incrementally */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 0;

        if (i == 0) {
            val = 1;                                /* a(0) = 1 */
        } else {
            /* 18 * a(i-1) + ... + 18 * a(i-9) */
            for (idx_t j = 1; j <= 9; ++j)
                if (i >= j)
                    val += (seq_t)18 * cache[i - j];

            /* -171 * a(i-10)  (only for i ≥ 10) */
            if (i >= 10)
                val -= (seq_t)171 * cache[i - 10];

            /* add the constant term p_i */
            if (i <= 9)   val += 2;
            else if (i == 10) val += 1;
            /* else add 0 for i > 10 */
        }

        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
