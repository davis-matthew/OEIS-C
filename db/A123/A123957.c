/*
 * A123957  Expansion of g.f.: x^4/((1+2*x) * (1-2*x+x^2+2*x^3)).
 * https://oeis.org/A123957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 36, i64 n <= 68, i128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=3*a(n‑2)−4*a(n‑3)−4*a(n‑4),
   a(1)=0, a(2)=0, a(3)=0, a(4)=1 */
#include <stdlib.h>
#include <string.h>

/* External types defined by the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation array (index 0 unused) */
    static idx_t  cache_size = 0; /* allocated size of cache */
    static idx_t  last_computed = 0; /* highest n that has been computed */

    /* Expand the cache if necessary. */
    if (cache == NULL || cache_size <= n) {
        size_t new_size = (size_t)n + 1;            /* need indices 0..n */
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed – return zero as a safe fallback. */
            return (seq_t)0;
        }
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Initialise base values if this is the first call. */
    if (last_computed == 0) {
        idx_t init_limit = cache_size > 0 ? cache_size - 1 : 0;
        if (init_limit >= 1) cache[1] = (seq_t)0;
        if (init_limit >= 2) cache[2] = (seq_t)0;
        if (init_limit >= 3) cache[3] = (seq_t)0;
        if (init_limit >= 4) cache[4] = (seq_t)1;
        last_computed = init_limit;
    }

    /* If the requested n has already been computed, just return it. */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute values up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 4) {
            cache[i] = (seq_t)1;
        } else if (i <= 3) {
            cache[i] = (seq_t)0;
        } else {
            cache[i] = (seq_t)3 * cache[i - 2]
                       - (seq_t)4 * cache[i - 3]
                       - (seq_t)4 * cache[i - 4];
        }
    }
    last_computed = n;
    return cache[n];
}
