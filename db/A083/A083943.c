/*
 * A083943  A generalized Jacobsthal sequence.
 * https://oeis.org/A083943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2, a(0) = 0, a(1) = 1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A083943 (“Generalized Jacobsthal sequence”).  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static idx_t cache_len = 0;      /* number of values stored (index+1) */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static seq_t *cache = NULL;      /* array of cached values */

    /* Grow cache if needed */
    if (n >= cache_cap) {
        /* ensure capacity covers n */
        idx_t new_cap = cache_cap ? cache_cap * 2 : 256;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure */
            /* oslst as per build spec: no errors, but we fall back */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first two values if not already present */
    if (cache_len <= 1) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len <= 2) {
        cache[1] = 1;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] + (seq_t)2 * cache[i-2] - (seq_t)2;
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
