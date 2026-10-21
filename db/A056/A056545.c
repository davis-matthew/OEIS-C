/*
 * A056545  a(n) = 4*n*a(n-1) + 1 with a(0)=1.
 * https://oeis.org/A056545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n‑th term of A056545; cache is rebuilt as needed. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms: cache[0..cache_size-1] valid. */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;          /* number of valid entries */

    /* First call: initialise with a(0) = 1. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* OOM – unlikely but safe */
        cache[0] = (seq_t)1;              /* a(0) */
        cache_size = 1;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_size) return cache[n];

    /* Extend the cache to hold indices up to n. */
    idx_t old_size = cache_size;           /* first new index to compute */
    cache_size = n + 1;                    /* new size in elements      */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
    if (!tmp) return (seq_t)0;             /* OOM – return 0 */
    cache = tmp;

    /* Compute the missing terms (old_size … n). */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t prev   = cache[i-1];
        seq_t mult   = (seq_t)4 * (seq_t)i;     /* 4*i as seq_t          */
        cache[i] = prev * mult + (seq_t)1;      /* recurrence 4*i*a[i-1]+1 */
    }

    return cache[n];
}
