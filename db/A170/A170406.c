/*
 * A170406  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(n) + 11 * S(n) - 66 * a(n-44) where g(0) = 1, g(1..43)
 *            = 2, g(44) = 1, g(n>=45) = 0 S(n) = sum_{i=1}^{min(n,43)} a(n-i)
 *            a(n) = 0 for n < 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and running window sum for efficient memoisation */
    static seq_t *cache = NULL;      /* sequence values */
    static size_t cache_size = 0;    /* number of computed terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t sum_last43 = 0;     /* sum of the last up to 43 terms
                                      * of the most recently computed index  */

    /* Ensure cache has at least one element (a(0) = 1) */
    if (cache_size == 0) {
        size_t init_cap = 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_size = 1;
        cache_cap = init_cap;
        sum_last43 = cache[0];        /* sum for next index (a(1)) */
    }

    /* Grow cache as needed */
    if (n >= cache_size) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms up to n */
        sz:
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term44 = 0;
            seq_t g = 0;

            if (i == 44) {
                g = 1;
                term44 = cache[0];
            } else if (i >= 1 && i <= 43) {
                g = 2;
            } /* else g remains 0 for i >= 45 */

            /* a(i) = g + 11 * sum_last43 - 66 * term44 */
            seq_t val = g + (seq_t)11 * sum_last43 - (seq_t)66 * term44;
            cache[i] = val;

            /* Update sum for next index: 
               sum_last43_new = sum_last43_old + a[i] - (i-43 >= 0 ? a[i-43] : 0) */
            sum_last43 += cache[i];
            if (i >= 43) {
                sum_last43 -= cache[i - 43];
            }
        }

        cache_size = n + 1;
    }

    return cache[n];
}
