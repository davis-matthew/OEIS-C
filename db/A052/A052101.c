/*
 * A052101  One of the three sequences associated with the polynomial x^3 - 2.
 * https://oeis.org/A052101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052101: a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3)
   with initial values a(0) = a(1) = a(2) = 1. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t computed = 0;    /* highest index computed, initial 2 */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 4; /* at least 0..3 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        /* cache[3] will be computed if needed */
        computed = 2;
    }

    /* if requested term already computed, return it */
    if ((size_t)n <= computed) {
        return cache[(size_t)n];
    }

    /* ensure the array is large enough for n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed, return zero */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms from computed+1 up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 - (seq_t)3 * cache[i-2]
                 + (seq_t)3 * cache[i-3];
    }
    computed = (size_t)n;
    return cache[(size_t)n];
}
