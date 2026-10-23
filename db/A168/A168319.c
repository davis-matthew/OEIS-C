/*
 * A168319  a(-1)=0. a(n)=a(n-1)^2-2^n.
 * https://oeis.org/A168319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)^2 - 2^n, with a(-1) = 0 (so a(0) = -1)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A168319.
 *  seq_t: integer type of the build (possibly arbitrary‑precision).
 *  idx_t: unsigned integer index type (uint64_t). */
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;   /* number of elements currently cached */

    /* initialise cache for the first term a(0) = -1 */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure fall‑back */
        cache[0]   = (seq_t)-1;             /* a(0) */
        cache_len  = 1;
    }

    /* if requested index already cached, just return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache up to index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;              /* allocation failure fall‑back */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t pow2   = (seq_t)1 << i;     /* 2^i, relying on shift for seq_t */
        cache[i] = prev * prev - pow2;
    }

    cache_len = new_len;
    return cache[n];
}
