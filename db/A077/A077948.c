/*
 * A077948  Expansion of 1/(1-x-x^2+2*x^3).
 * https://oeis.org/A077948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-3) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2
 * Range:     i8 n <= 18, i16 n <= 41, i32 n <= 86, i64 n <= 173,
 *            i128 n <= 347, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_size = 0;    /* number of cached elements */

/* Ensure that cache has entries up to index n, computing new ones if needed. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                      /* already computed */

    /* at least store up to n, but reserve first 3 for base values */
    size_t new_size = n + (size_t)1;
    if (new_size < 3)
        new_size = 3;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)                 /* allocation failure: terminate */
        exit(1);

    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else
            cache[i] = cache[i-1] + cache[i-2] - (seq_t)2 * cache[i-3];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
