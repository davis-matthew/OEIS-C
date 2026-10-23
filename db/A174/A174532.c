/*
 * A174532  Expansion of x^5/((1-x)*(1+x-x^5)).
 * https://oeis.org/A174532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-5) – a(n-6)
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values       */
static size_t cache_len = 0;     /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* Initialise the cache with the first six terms when first called. */
    if (!cache)
    {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();        /* abort on allocation failure         */
        cache[0] = 0; cache[1] = 0; cache[2] = 0;
        cache[3] = 0; cache[4] = 0; cache[5] = 1;
    }

    /* If n is already cached, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Need to extend the cache.  Remember the old size, resize,
       initialise new entries to zero, then compute new terms. */
    size_t old_len  = cache_len;
    size_t new_len  = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    for (size_t i = old_len; i < new_len; ++i)
        cache[i] = 0;

    cache_len = new_len;

    /* Compute terms from old_len through n inclusive. */
    for (size_t i = old_len; i <= (size_t)n; ++i)
    {
        /* The recurrence requires a(i-2), a(i-5), a(i-6); all
           are available because i ≥ 6 in this loop.       */
        cache[i] = cache[i-2] + cache[i-5] - cache[i-6];
    }

    return cache[(size_t)n];
}
