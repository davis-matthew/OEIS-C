/*
 * A163445  a(n) = 14*a(n-1) - 47*a(n-2) for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A163445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 47*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise the cache with the first two terms */
    if (cache_len == 0) {
        cache_len = 2;                                    /* at least a(0) and a(1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;  /* allocation failure – unlikely in tests */
        cache[0] = 1;
        cache[1] = 9;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Otherwise extend the cache up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return 0;            /* allocation failure – unlikely in tests */
    cache = tmp;

    for (idx_t i = cache_len; i < new_len; ++i) {
        cache[i] = 14 * cache[i - 1] - 47 * cache[i - 2];
    }
    cache_len = new_len;

    return cache[n];
}
