/*
 * A080040  a(n) = 2*a(n-1) + 2*a(n-2) for n > 1; a(0)=2, a(1)=2.
 * https://oeis.org/A080040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(0)=2, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first two terms */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;          /* allocation failed – unlikely in tests */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
    }

    /* return cached value if already known */
    if (n < cache_size)
        return cache[n];

    /* extend the cache up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) return (seq_t)0;                  /* allocation failed – unlikely in tests */
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        /* 2*cache[i-1] + 2*cache[i-2] */
        cache[i] = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-2];
    }

    cache_size = new_size;
    return cache[n];
}
