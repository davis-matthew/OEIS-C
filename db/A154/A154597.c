/*
 * A154597  a(n) = 15*a(n-1) + a(n-2) with a(0) = 0, a(1) = 1.
 * https://oeis.org/A154597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    if (cache_size <= (size_t)n) {
        /* need to enlarge cache */
        size_t new_size = (cache_size == 0 ? 4 : cache_size * 2);
        while (new_size <= (size_t)n)
            new_size *= 2;

        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_cache)
            abort();  /* allocation failure */

        /* copy existing data */
        if (cache) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_size);
            free(cache);
        }

        /* initialise the new part of the cache */
        if (cache_size == 0) {
            /* base terms */
            new_cache[0] = (seq_t)0;
            if (new_size > 1)
                new_cache[1] = (seq_t)1;
            for (size_t i = 2; i < new_size; ++i)
                new_cache[i] = new_cache[i - 1] * (seq_t)15 + new_cache[i - 2];
        } else {
            /* cache already has values up to cache_size-1 */
            for (size_t i = cache_size; i < new_size; ++i)
                new_cache[i] = new_cache[i - 1] * (seq_t)15 + new_cache[i - 2];
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
