/*
 * A228208  y-values in the solution to x^2 - 20*y^2 = 176.
 * https://oeis.org/A228208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 3*a(n-2) – a(n-4),
   initial values a(1)=1, a(2)=2, a(3)=5, a(4)=7.  */

/* The sequence is stored in a 1‑based cache. */
seq_t a(idx_t n) {
    /* Offset is 1; the behaviour for n = 0 does not matter but we
       return 0 safely. */
    if (n == 0) {
        return 0;
    }

    /* Initial values for the first four terms. */
    static const seq_t init[4] = { 1, 2, 5, 7 };

    /* Static cache and bookkeeping. */
    static seq_t *cache = NULL;   /* pointer to the allocated array */
    static size_t cache_len = 0;  /* number of elements allocated */
    static idx_t max_computed = 0;/* highest index that has been filled */

    /* Ensure the buffer is large enough for n terms. */
    if (cache_len < (size_t)n) {
        size_t new_len = cache_len ? cache_len << 1 : 4; /* double, or at least 4 */
        if (new_len < (size_t)n) {
            new_len = n;
        }
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        /* Zero‑initialize the new area.  For built‑in unsigned types this
           is not strictly required but keeps the logic simple. */
        for (size_t i = cache_len; i < new_len; ++i) {
            cache[i] = 0;
        }
        cache_len = new_len;
    }

    /* If we already computed up to n, just return the value. */
    if (n <= (idx_t)max_computed) {
        return cache[n - 1];
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i - 1] = init[i - 1];
        } else {
            /* a(i) = 3*a(i-2) – a(i-4) */
            cache[i - 1] = 3 * cache[i - 3] - cache[i - 5];
        }
    }

    max_computed = n;
    return cache[n - 1];
}
