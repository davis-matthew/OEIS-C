/*
 * A144746  a(n) = a(n-1)^2 - a(n-1) - 1, a(0)=6.
 * https://oeis.org/A144746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-1) - 1, a(0) = 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A144746.
 * The function uses memoisation with a static cache so that
 * calls with arbitrary n values are answered efficiently.
 * All arithmetic is performed in the seq_t type, which may
 * be an unsigned integer type of any size or an
 * arbitrary-precision integer class that supports the usual
 * arithmetic operators. */
seq_t a(idx_t n)
{
    /* static storage for previously computed terms */
    static seq_t *cache = NULL;   /* array of terms a(0) .. a(cache_n) */
    static idx_t cache_n = 0;     /* highest index stored in cache */

    /* special handling for the first term */
    if (n == 0) {
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (cache == NULL) return (seq_t)0;   /* allocation failure */
            cache[0] = (seq_t)6;
        }
        cache_n = 0;
        return cache[0];
    }

    /* Ensure the cache is large enough for index n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)6;
        cache_n = 0;
    }

    /* Grow the cache and compute missing terms up to n */
    if (n > cache_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        for (idx_t i = cache_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            /* a(i) = prev*prev - prev - 1  */
            cache[i] = prev * prev - prev - (seq_t)1;
        }

        cache_n = n;
    }

    return cache[n];
}
