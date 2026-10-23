/*
 * A164907  a(n) = (3*3^n - (-1)^n)/2.
 * https://oeis.org/A164907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*3^n - (-1)^n)/2 * Recurrence: a(0)=1, a(1)=5,
 *            a(n)=2*a(n-1)+3*a(n-2) for n>1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of cached values */
static size_t   cache_cap = 0;    /* allocated capacity */
static idx_t    cache_size = 0;   /* number of values currently stored */

/* Extend cache to hold values up to `n` and compute missing terms. */
static void ensure_cache(idx_t n)
{
    /* Extend storage if necessary. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the very first terms if the cache is empty. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i >= 2 here */
        seq_t val = (seq_t)(2 * cache[i - 1] + 3 * cache[i - 2]);
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
