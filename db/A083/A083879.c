/*
 * A083879  a(0)=1, a(1)=4, a(n) = 8*a(n-1) - 14*a(n-2), n >= 2.
 * https://oeis.org/A083879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2); a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the cache with the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) {
            abort();
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* Return a term that is already cached */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache and compute missing terms */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (new_cache == NULL) {
        abort();
    }
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t mul1 = cache[i - 1] * (seq_t)8;
        seq_t mul2 = cache[i - 2] * (seq_t)14;
        cache[i] = mul1 - mul2;
    }

    cache_size = new_size;
    return cache[n];
}
