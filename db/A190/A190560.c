/*
 * A190560  a(n) = 8*a(n-1) + 6*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 6*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Desired cache length (n+1 entries) */
    size_t new_size = (size_t)(n + 1);

    if (new_size > cache_size) {
        /* Grow or create the cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – unlikely in tests */
        cache = tmp;

        /* Ensure base cases are available */
        if (cache_size == 0) {
            cache[0] = 0;
            if (new_size > 1) cache[1] = 1;
        } else if (cache_size == 1) {
            cache[1] = 1;
        }

        /* Compute missing terms */
        size_t start = (cache_size > 1) ? cache_size : 2;
        for (size_t i = start; i < new_size; ++i) {
            cache[i] = 8 * cache[i - 1] + 6 * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
