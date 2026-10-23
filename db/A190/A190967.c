/*
 * A190967  a(n) = 4*a(n-1) - 9*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 20, i64 n <= 41, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) - 9*a(n-2)
 * Initial values: a(0) = 0, a(1) = 1
 *
 * Computes the terms of OEIS sequence A190967.
 * Uses a static cache so that calls in any order are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache: cache[i] = a(i) for i < count */
    static seq_t *cache = NULL;
    static idx_t count = 0; /* number of terms stored */
    static idx_t cap = 0;   /* capacity of the cache array */

    /* Initialise cache with first two terms if needed */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* out of memory: undefined, but safe fallback */
        cache[0] = 0;
        cache[1] = 1;
        count = 2;
    }

    /* If n is already computed, return it */
    if (n < count)
        return cache[n];

    /* Extend the cache up to n */
    if (n >= cap) {
        /* Grow capacity to at least n+1 */
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed; safe fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new terms */
    for (idx_t i = count; i <= n; ++i) {
        cache[i] = (cache[i-1] * 4) - (cache[i-2] * 9);
    }
    count = n + 1;

    return cache[n];
}
