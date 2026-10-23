/*
 * A181880  Expansion of 1/(1-4*x-3*x^2-x^3).
 * https://oeis.org/A181880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) + a(n-3), with a(0)=1, a(1)=4,
 *            a(2)=19
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, size_t */

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of cache entries valid */

/* Grow the cache capacity to hold at least 'needed' indices. */
static void grow_cache(size_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap <= needed)
        new_cap = needed + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* safe cast for indexing */

    grow_cache(N);

    /* Compute all missing terms up to N. */
    for (size_t i = cache_len; i <= N; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 4;
        else if (i == 2)
            cache[i] = 4 * cache[1] + 3 * cache[0]; /* a(-1)=0 */
        else
            cache[i] = 4 * cache[i-1] + 3 * cache[i-2] + cache[i-3];
    }
    cache_len = N + 1;             /* all terms up to N are now valid */

    return cache[N];
}
