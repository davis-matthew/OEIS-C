/*
 * A145103  a(0) = a(1) = 1. a(n+1) = ceiling(n*a(n)/a(n-1)), for n >= 1.
 * https://oeis.org/A145103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = a(1) = 1
   For n ≥ 1: a(n+1) = ceil( n * a(n) / a(n-1) ) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n < 2)
        return (seq_t)1;

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t last_seen = 1;       /* highest index computed so far */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_cap = 2;
        last_seen = 1;
    }

    /* Grow cache if necessary to contain index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the term, return it */
    if (n <= last_seen)
        return cache[n];

    /* Compute consecutive missing terms up to n */
    for (idx_t i = last_seen + 1; i <= n; ++i) {
        /* numerator = (i-1) * a[i-1] */
        idx_t prod = (i - 1) * (idx_t)cache[i - 1];
        seq_t numerator = (seq_t)prod;

        seq_t denominator = cache[i - 2];
        /* ceil division: (numerator + denominator - 1) / denominator */
        cache[i] = (numerator + denominator - 1) / denominator;
    }

    last_seen = n;
    return cache[n];
}
