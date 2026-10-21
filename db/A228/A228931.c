/*
 * A228931  Optimal ascending continued fraction expansion of sqrt(2)-1.
 * https://oeis.org/A228931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, cofr, easy
 * Formula:   a(n) = a(n-1)^2 - 2, with a(1) = 2, a(2) = -6
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A228931: Optimal ascending continued fraction expansion of sqrt(2)-1.
 * This sequence satisfies a(1)=2, a(2)=-6, and for n>2:
 *   a(n) = a(n-1)^2 - 2
 */
seq_t a(idx_t n) {
    /* Handle the two base cases immediately, without allocating any cache. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)(-6);

    /* Static cache for already computed values.  cache[i] stores a(i+1). */
    static seq_t *cache = NULL;       /* Pointer to the cache array. */
    static size_t cache_len = 0;      /* Number of elements currently stored. */
    static size_t cache_cap = 0;      /* Allocated capacity of the cache. */

    /* Allocate and initialise the cache if it does not exist yet. */
    if (cache == NULL) {
        cache_cap = 4;                /* Small initial capacity. */
        cache_len = 2;                /* We store a(1) and a(2). */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;         /* Out‑of‑memory; caller will treat 0 as error. */
        cache[0] = (seq_t)2;          /* a(1) */
        cache[1] = (seq_t)(-6);       /* a(2) */
    }

    /* Expand the cache so it contains at least n elements. */
    if (cache_len < (size_t)n) {
        /* Grow capacity if necessary (doubling strategy). */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* Out‑of‑memory; propagate failure. */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev * prev - (seq_t)2;
        }
        cache_len = (size_t)n;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
