/*
 * A131662  Numbers n where either n or n+1 is divisible by the numbers from 1
 *          to 12.
 * https://oeis.org/A131662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-8) - a(n-9) for n > 9 Initial values (n = 1
 *            … 9) are taken from the OEIS entry
 * Range:     u8 none, u16 n <= 17, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (undefined by OEIS). */
    if (n < 1) return (seq_t)0;

    /* Hard‑coded first 9 terms. */
    static const seq_t init[9] = {
        (seq_t)2519,
        (seq_t)11879,
        (seq_t)13320,
        (seq_t)14399,
        (seq_t)15840,
        (seq_t)25200,
        (seq_t)27719,
        (seq_t)27720,
        (seq_t)30239
    };

    /* Cached sequence values.  Cache grows lazily on demand. */
    static seq_t *cache   = NULL;   /* array of seq_t values */
    static idx_t  sz      = 0;      /* current number of filled entries */
    static idx_t  cap     = 0;      /* current allocation capacity */

    /* Handle the first 9 terms directly. */
    if (n <= 9) return init[n-1];

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cap = 9;
        sz  = 9;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        for (idx_t i = 0; i < 9; ++i) cache[i] = init[i];
    }

    /* Grow the cache until it contains at least n terms. */
    while (sz < n) {
        /* Expand capacity if necessary. */
        if (sz >= cap) {
            idx_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;   /* allocation failure */
            cache = new_cache;
            cap   = new_cap;
        }

        /* Compute the next term using the recurrence. */
        idx_t idx   = sz + 1;                           /* 1‑based index of new term */
        seq_t val   = cache[idx-2]                      /* a(idx-1) */
                    + cache[idx-9]                      /* a(idx-8) */
                    - cache[idx-10];                    /* a(idx-9) */
        cache[sz]   = val;
        ++sz;
    }

    return cache[n-1];
}
