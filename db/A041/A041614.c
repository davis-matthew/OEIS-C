/*
 * A041614  Numerators of continued fraction convergents to sqrt(326).
 * https://oeis.org/A041614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 650*a(n-2)-a(n-4) initial values: a(0)=18, a(1)=325,
 *            a(2)=11718, a(3)=211249
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t  cache_len = 0;     /* number of valid entries in cache */

/* Ensure the cache contains values up to index n.
   The function expands the cache when necessary and
   computes missing terms using the recurrence. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                                 /* already have it */

    /* 1) grow capacity if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 4;
        while (new_cap < needed)
            new_cap <<= 1;                       /* double */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)                          /* out-of-memory */
            abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* 2) initialise first four terms if not yet done */
    if (cache_len < 4) {
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)325;
        cache[2] = (seq_t)11718;
        cache[3] = (seq_t)211249;
        cache_len = 4;
    }

    /* 3) compute any missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v2 = cache[i - 2];
        seq_t v4 = cache[i - 4];
        cache[i] = (seq_t)650 * v2 - v4;          /* recurrence */
    }
    cache_len = n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
