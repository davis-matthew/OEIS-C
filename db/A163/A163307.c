/*
 * A163307  a(n) = 14*a(n-1) - 44*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A163307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 44*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for memoization */
static seq_t *cache = NULL;           /* pointer to stored terms      */
static idx_t  cache_len = 0;          /* number of terms computed    */
static size_t cache_cap = 0;          /* current allocation capacity */

/* Ensure the cache has space for at least 'needed' terms.  */
static void
ensure_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_cap)
        return;

    size_t new_cap = (size_t)needed;
    seq_t *new_cache;

#ifdef __cplusplus
    /* C++ build – use new/delete so that constructors/destructors run */
    new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    /* C build – simple malloc/memcpy is sufficient for POD types */
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache && cache_len)
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
#endif

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Main function implementing the OEIS sequence A163307. */
seq_t
a(idx_t n)
{
    /* Initialise base cases on first use */
    if (cache_len == 0)
    {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* Return cached value if available */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate term n */
    ensure_capacity(n + 1);

    /* Compute missing terms iteratively */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t t1 = (seq_t)14 * cache[i - 1];
        seq_t t2 = (seq_t)44 * cache[i - 2];
        cache[i] = t1 - t2;
    }

    cache_len = n + 1;
    return cache[n];
}
