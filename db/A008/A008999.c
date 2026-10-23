/*
 * A008999  a(n) = 2*a(n-1) + a(n-4).
 * https://oeis.org/A008999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-4), a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t  cache_len = 0;         /* number of terms stored (max index + 1) */

/* Expand the cache to at least index n (inclusive) and fill new entries */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                     /* already computed up to n */

    idx_t new_len = n + 1;           /* need indices 0 .. n */
    seq_t *new_cache;

#ifdef __cplusplus
    /* C++: use new[] to honour constructors/destructors */
    new_cache = new seq_t[new_len];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    /* C: realloc is sufficient for POD types */
    new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                    /* allocation failure */
#endif

    cache = new_cache;

    /* Compute new entries */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 2;
        else if (i == 2)
            cache[i] = 4;
        else if (i == 3)
            cache[i] = 8;
        else
            cache[i] = 2 * cache[i - 1] + cache[i - 4];
    }

    cache_len = new_len;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
