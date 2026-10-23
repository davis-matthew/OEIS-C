/*
 * A016273  Expansion of g.f. 1/((1-2x)(1-3x)(1-5x)).
 * https://oeis.org/A016273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 31*a(n-2) + 30*a(n-3), a(0)=1, a(1)=10,
 *            a(2)=69
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* pointer to the cached terms */
static size_t cache_cap = 0;     /* capacity of the array */
static size_t cache_len = 0;     /* number of elements currently stored */

/* Computes a(n) for the sequence A016273 */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call */
    if (cache_len == 0) {
        /* initial capacity fits the first three terms */
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)69;
        cache_len = 3;
    }

    /* Return the value immediately if it is already cached */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Expand the cache and compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Ensure the cache has enough space */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 3;
            while (new_cap <= i) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence: a(i) = 10*a(i-1) - 31*a(i-2) + 30*a(i-3) */
        seq_t val = cache[i-1] * (seq_t)10
                  - cache[i-2] * (seq_t)31
                  + cache[i-3] * (seq_t)30;
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
