/*
 * A077412  Chebyshev U(n,x) polynomial evaluated at x=8.
 * https://oeis.org/A077412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2), a(0) = 1, a(1) = 16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>          /* for memset */

/* cache of already computed terms */
static seq_t *cache      = NULL;
static idx_t  cache_cap  = 0;       /* allocated length of cache   */
static idx_t  cache_last = 0;       /* highest index whose value
                                      has been computed        */

/* Expand the cache so that indices 0..n are writable.
   Newly allocated cells are zero‑initialised. */
static void ensure_cache(idx_t n)
{
    if (cache && cache_cap > n)
        return;

    idx_t old_cap = cache_cap;

    /* ensure a minimal capacity of 2 (for a(0), a(1)) and grow
       geometrically until we can hold index n */
    if (cache_cap < 2)
        cache_cap = 2;
    while (cache_cap <= n)
        cache_cap *= 2;

    seq_t *ptr;
    if (cache == NULL)
        ptr = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    else
        ptr = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    if (!ptr)
        exit(1);                    /* allocation failure */

    if (cache == NULL) {
        /* freshly allocated block – zero it */
        memset(ptr, 0, sizeof(seq_t) * cache_cap);
    } else if (cache_cap > old_cap) {
        /* newly added tail – zero it */
        memset(ptr + old_cap, 0, sizeof(seq_t) * (cache_cap - old_cap));
    }

    cache = ptr;

    /* initialise the base cases once */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)16;
    cache_last = 1;
}

/* A077412 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)16;

    ensure_cache(n);

    if (n <= cache_last)
        return cache[n];

    /* compute terms that are still unknown */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t prev1 = cache[i-1];
        seq_t prev2 = cache[i-2];
        cache[i] = (seq_t)16 * prev1 - prev2;
    }
    cache_last = n;
    return cache[n];
}
