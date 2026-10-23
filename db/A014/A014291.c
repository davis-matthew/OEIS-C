/*
 * A014291  Imaginary Rabbits: imaginary part of a(0)=i; a(1)=-i; a(n) =
 *          a(n-1) + i*a(n-2), with i = sqrt(-1).
 * https://oeis.org/A014291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4), a(0)=1, a(1)=a(2)=a(3)=-1
 *            (Imaginary Rabbits sequence A014291)
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 63, i64 n <= 121,
 *            i128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t  cache_size = 0; /* number of elements currently allocated */

/* Initialise the first four terms. */
static void init_cache(void)
{
    cache_size = 4;
    cache = (seq_t *)malloc(4 * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = 1;
    cache[1] = -1;
    cache[2] = -1;
    cache[3] = -1;
}

/* Ensure cache is large enough to contain indices 0 … n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        init_cache();
    }
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* old_size holds the cache size before we possibly grow it. */
    idx_t old_size = cache_size;
    ensure_cache(n);

    /* Start computing from the first index that is not yet known. */
    if (old_size < 4)
        old_size = 4;

    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 2] - cache[i - 4];
    }

    return cache[n];
}
