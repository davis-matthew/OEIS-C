/*
 * A041265  Denominators of continued fraction convergents to sqrt(145).
 * https://oeis.org/A041265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 24*a(n-1) + a(n-2), a(0)=1, a(1)=24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of items actually stored */

    /* initialise cache with the first two terms */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
    }

    /* compute up to n if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        /* grow the cache */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        /* fill new entries using the recurrence */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = ((seq_t)24) * cache[i - 1] + cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
