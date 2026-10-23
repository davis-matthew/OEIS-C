/*
 * A169087  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n≥1: a(n)=24*∑_{k=1}^{24} a(n−k)−300*a(n−25)+p(n),
 *            where p(n)=2 for 1≤n≤24, p(25)=1, and p(n)=0 for n>25
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;  /* highest index already computed */
    static size_t cache_capacity = 0;

    /* Ensure cache is allocated with enough space */
    if (cache == NULL) {
        cache_capacity = (size_t)n + 1;
        cache = seq_alloc_array(cache_capacity);
        cache[0] = (seq_t)1;
        max_computed = 0;
    } else if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (idx_t i = 0; i <= max_computed; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Return immediately if we already have the requested value */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Initialise sum of the last 24 already computed terms
       (used as the sliding window when computing new values). */
    seq_t sum_last_24 = 0;
    /* sum_last_24 currently contains a_{max_computed} + … + a_{max_computed-23} */
    idx_t adj = max_computed + 1;          /* next index to be computed   */
    for (int j = 1; j <= 24; ++j) {
        if (adj < (idx_t)j) break;
        sum_last_24 += cache[adj - j];
    }

    /* Compute values for all indices from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)24 * sum_last_24;

        if (i >= 25) {
            val = val - ((seq_t)300 * cache[i - 25]);
        }

        /* add the appropriate constant term p(i) */
        if (i <= 24) {
            val = val + (seq_t)2;
        } else if (i == 25) {
            val = val + (seq_t)1;
        }

        cache[i] = val;

        /* update sliding window for the next iteration */
        sum_last_24 += val;
        if (i >= 24) {
            sum_last_24 -= cache[i - 24];
        }
    }

    max_computed = n;
    return cache[n];
}
