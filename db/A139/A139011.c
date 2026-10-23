/*
 * A139011  Real part of (2 + i)^n, where i = sqrt(-1).
 * https://oeis.org/A139011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 54, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first two terms if necessary */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return (seq_t)0;          /* out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* Return already computed value */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to accommodate values up to index n */
    idx_t new_len = n + (idx_t)1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (tmp == NULL)
        return (seq_t)0;                            /* out‑of‑memory guard */
    cache = tmp;
    idx_t old_len = cache_len;
    cache_len = new_len;

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_len; i < cache_len; ++i) {
        cache[i] = (seq_t)4 * cache[i - 1] - (seq_t)5 * cache[i - 2];
    }

    return cache[n];
}
