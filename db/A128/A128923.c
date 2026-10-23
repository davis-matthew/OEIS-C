/*
 * A128923  a(1) = a(2) = 1. a(n) = number of terms among
 *          (a(1),a(2),...a(n-1)) that divide (a(n-1) + a(n-2)).
 * https://oeis.org/A128923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128923:
   a(1) = a(2) = 1.
   For n > 2, a(n) = number of indices i < n such that a(i) | (a(n‑1) + a(n‑2)). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1‑based static cache of computed values */
    static seq_t *cache   = NULL;      /* cache[0] unused, cache[1]..cache[max_n] defined */
    static idx_t  cache_cap = 0;       /* allocated capacity (>= max_n+1) */
    static idx_t  max_cached_n = 0;    /* maximum n for which cache is filled */

    /* Ensure the cache can hold index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;   /* start with 8, grow by factor 2 */
        while (new_cap <= n) new_cap <<= 1;

        /* Allocate new array */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) {
            /* allocation failed – abort to avoid undefined behaviour */
            abort();
        }

        /* Copy existing values */
        for (idx_t i = 0; i <= max_cached_n; ++i) {
            new_cache[i] = cache ? cache[i] : (seq_t)0;
        }

        /* Free old array if any */
        if (cache) {
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }

        cache      = new_cache;
        cache_cap   = new_cap;
    }

    /* Compute and store any missing values up to n */
    for (idx_t k = max_cached_n + 1; k <= n; ++k) {
        if (k <= 2) {
            cache[k] = (seq_t)1;
        } else {
            seq_t sum   = cache[k-1] + cache[k-2];
            seq_t count = (seq_t)0;
            for (idx_t i = 1; i < k; ++i) {
                /* In both C and C++ a%b with seq_t is defined for non‑zero b */
                if (sum % cache[i] == (seq_t)0) {
                    ++count;
                }
            }
            cache[k] = count;
        }
    }

    max_cached_n = n;
    return cache[n];
}
