/*
 * A158580  a(n) = Hermite(n, 15).
 * https://oeis.org/A158580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 30*a(n-1) - 2*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 26,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 553 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms: a(0) = 1, a(1) = 30 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* the memoization array          */
    static size_t  cap    = 0;      /* current allocated capacity     */
    static idx_t   max_n  = 1;      /* highest index that is already computed */

    /* Handle the two trivial early indices directly. */
    if (n < 2) {
        return (n == 0) ? (seq_t)1 : (seq_t)30;
    }

    /* Lazily allocate the cache the first time the function is called. */
    if (cache == NULL) {
        /* Ensure space for indices 0 and 1. */
        ensure_cache(1, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
    }

    /* If the requested term hasn't been computed yet, extend the cache. */
    if (n > max_n) {
        ensure_cache(n, &cache, &cap);

        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* 30 * a(k-1) */
            seq_t term1 = (seq_t)30 * cache[k - 1];
            /* 2 * (k-1) * a(k-2) */
            seq_t factor = 2 * (seq_t)(k - 1);
            seq_t term2 = factor * cache[k - 2];
            /* a(k) = term1 - term2 */
            cache[k] = term1 - term2;
        }

        max_n = n;
    }

    return cache[n];
}
