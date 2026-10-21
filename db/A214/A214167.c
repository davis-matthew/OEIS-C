/*
 * A214167  Number of 0..5 colorings of a 1 X (n+1) array circular in the n+1
 *          direction with new values 0..5 introduced in row major order.
 * https://oeis.org/A214167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 30*a(n-2) + 20*a(n-3) + 31*a(n-4) -
 *            30*a(n-5), with a(1)=1, a(2)=1, a(3)=4, a(4)=11, a(5)=41
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t alloc_size = 0;

    /* Initialize cache with the first five terms once. */
    if (cache_size == 0) {
        size_t init_cap = 10;               /* buffer space beyond the first 5 */
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        if (!cache) return 0;
        alloc_size = init_cap;
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)1;   /* a(2) */
        cache[2] = (seq_t)4;   /* a(3) */
        cache[3] = (seq_t)11;  /* a(4) */
        cache[4] = (seq_t)41;  /* a(5) */
        cache_size = 5;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_size) {
        return cache[n-1];
    }

    /* Ensure enough space in the cache. */
    if ((size_t)n > alloc_size) {
        size_t new_cap = alloc_size;
        while ((size_t)n > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        alloc_size = new_cap;
    }

    /* Compute terms from the next missing index up to n. */
    for (idx_t k = (idx_t)(cache_size + 1); k <= n; ++k) {
        size_t i = (size_t)(k - 1);        /* array index for a(k) */
        seq_t val = 10 * cache[i-1]
                  - 30 * cache[i-2]
                  + 20 * cache[i-3]
                  + 31 * cache[i-4]
                  - 30 * cache[i-5];
        cache[i] = val;
    }
    cache_size = (size_t)n;

    return cache[n-1];
}
