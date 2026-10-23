/*
 * A022129  Fibonacci sequence beginning 3, 20.
 * https://oeis.org/A022129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 20
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;

    /* Initialize cache if first call */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback on allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)20;
        computed_up_to = 1;
    }

    /* Grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size;
        if (new_size < 2) new_size = 2;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* fallback on allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        computed_up_to = n;
    }

    return cache[n];
}
