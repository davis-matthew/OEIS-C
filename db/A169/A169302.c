/*
 * A169302  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1; 
 * for 1 <= n <= 28:  a(n) = 2 + 47 * sum_{i=0}^{n-1} a(i);
 * for n >= 29:        a(n) = 47 * ( sum_{i=1}^{28} a(n-i) - 24 * a(n-29) ).
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* sequence values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t max_n = (idx_t)-1;   /* highest index that has been computed */
    static seq_t prefix_sum = 0;     /* sum of a[0..max_n] (used while building base terms) */
    static bool initialized = false;

    /* Ensure storage exists */
    if (!initialized) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Build base terms up to min(28, n) */
        cache[0] = 1;
        prefix_sum = cache[0];
        idx_t limit = (n < 28) ? n : 28;
        for (idx_t k = 1; k <= limit; ++k) {
            cache[k] = 2 + 47 * prefix_sum;
            prefix_sum += cache[k];
        }
        max_n = limit;
        initialized = true;
    } else if ((size_t)n + 1 > cache_cap) { /* grow cache if needed */
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    if (max_n < n) {
        for (idx_t k = max_n + 1; k <= n; ++k) {
            if (k <= 28) {
                /* Base recurrence */
                cache[k] = 2 + 47 * prefix_sum;
                prefix_sum += cache[k];
            } else {
                /* Recurrence for n >= 29 */
                seq_t sum_last28 = 0;
                for (idx_t j = 1; j <= 28; ++j)
                    sum_last28 += cache[k - j];
                seq_t term = cache[k - 29];
                cache[k] = 47 * (sum_last28 - 24 * term);
            }
        }
        max_n = n;
    }

    return cache[n];
}
