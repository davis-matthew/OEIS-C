/*
 * A010050  a(n) = (2n)!.
 * https://oeis.org/A010050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2n)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache: cache[k] = (2k)! for 0 <= k < cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements stored */
    static size_t cache_capacity = 0; /* allocated size */

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        if (cache) {
            memcpy(tmp, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* If the requested n is already cached, return it */
    if (n < cache_size) return cache[n];

    /* Initialise cache[0] = 0! = 1 if it hasn't been yet */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute missing factorials up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t prod = (seq_t)(2*k - 1) * (seq_t)(2*k);
        cache[k] = cache[k-1] * prod;
    }
    cache_size = n + 1;

    return cache[n];
}
