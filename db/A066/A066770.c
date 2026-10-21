/*
 * A066770  a(n) = 5^n*sin(2n*arctan(1/2)) or numerator of
 *          tan(2n*arctan(1/2)).
 * https://oeis.org/A066770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = 6*a(n-1) - 25*a(n-2) with a(1)=4, a(2)=24 (n ≥ 1)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[i] holds a(i) for i≥1. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size of cache */
    static idx_t max_n = 0;   /* maximum n computed so far */

    /* Handle the first two terms explicitly.  They are needed
       even if the cache has not yet been allocated. */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)24;

    /* Ensure the cache exists and is large enough. */
    if (!cache) {
        /* Initially allocate space for a few terms. */
        cap   = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* memory failure – return 0 as safe‑fallback */
        /* Initialise first two terms in the cache. */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)24;
        max_n   = 2;
    }

    /* Expand the cache if necessary. */
    if (n > cap - 1) {
        size_t new_cap = cap;
        while (new_cap <= n) new_cap = new_cap * 2;          /* grow exponentially */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* memory failure – safe fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i-1] - (seq_t)25 * cache[i-2];
    }
    max_n = (idx_t) n;  /* update last computed index */

    return cache[n];
}
