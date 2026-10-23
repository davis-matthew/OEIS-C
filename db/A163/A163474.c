/*
 * A163474  a(n) = 16*a(n-1) - 61*a(n-2) for n > 1; a(0) = 3, a(1) = 27.
 * https://oeis.org/A163474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3, a(1) = 27 a(n) = 16*a(n-1) - 61*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache for computed terms */
    static size_t cache_len = 0;         /* number of cached entries */

    /* initialise the cache with the first two terms */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return 0;                    /* allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)27;
    }

    /* already computed ? */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow the cache to hold up to index n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return 0;                        /* allocation failure */
    cache = tmp;
    cache_len = new_len;

    /* compute missing terms via the recurrence */
    for (size_t i = old_len; i < new_len; ++i) {
        cache[i] = (seq_t)16 * cache[i - 1] - (seq_t)61 * cache[i - 2];
    }

    return cache[(size_t)n];
}
