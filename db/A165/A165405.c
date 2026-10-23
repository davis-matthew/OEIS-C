/*
 * A165405  a(n) = 6*a(n-2) - a(n-1), with a(0)=1, a(1)=3.
 * https://oeis.org/A165405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-1), a(0)=1, a(1)=3
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 21, i64 n <= 41, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static cache for memoisation: grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t n_idx = (size_t)n;   /* safer index type */

    /* Initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);                /* allocation failure */
        cache[0] = (seq_t)1;                /* a(0) */
        cache[1] = (seq_t)3;                /* a(1) */
    }

    /* Grow cache up to needed index */
    if (n_idx >= cache_cap) {
        size_t new_cap = n_idx + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);            /* allocation failure */
        /* Compute new terms using the recurrence */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            /* a(i) = 6*a(i-2) - a(i-1) */
            seq_t a_nm2 = new_cache[i-2];
            seq_t a_nm1 = new_cache[i-1];
            new_cache[i] = (seq_t)( (seq_t)6 * a_nm2 - a_nm1 );
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[n_idx];
}
