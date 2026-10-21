/*
 * A084158  a(n) = A000129(n) * A000129(n+1)/2.
 * https://oeis.org/A084158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3), a(0)=0, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t   *cache   = NULL;   /* cache of computed a(n) values */
static idx_t    cache_size = 0;   /* number of entries currently valid */
static idx_t    cache_cap  = 0;   /* allocated capacity (in elements) */

static void
ensure_capacity(idx_t n)
{
    /* Return if we already have space */
    if (n + 1 <= cache_cap)
        return;

    /* Choose a new capacity (double or enough for n+1) */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < n + 1)
        new_cap = n + 1;

    /* Allocate a new buffer */
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    /* In C++ the type seq_t may be a class, so use new[] */
    new_cache = new seq_t[new_cap];
#else
    /* In C we use realloc (void* → seq_t*) with a cast */
    new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
#endif
    if (!new_cache) {
        /* allocation failure – abort program as the OEIS tests expect a crash */
        exit(EXIT_FAILURE);
    }

    /* Copy existing values to the new buffer */
    if (cache && cache_size > 0) {
#ifdef __cplusplus
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        /* Nothing to do for C; realloc already preserves data */
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache has room for the requested index */
    ensure_capacity(n);

    /* Initialise the three starting values if the cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* Return a cached value if we already computed it */
    if (n < cache_size)
        return cache[n];

    /* Compute any missing terms up to n (inclusive) */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t t1 = cache[k - 1];
        seq_t t2 = cache[k - 2];
        seq_t t3 = cache[k - 3];
        cache[k] = t1 * (seq_t)5 + t2 * (seq_t)5 - t3;
    }
    cache_size = n + 1;

    return cache[n];
}
