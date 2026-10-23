/*
 * A049660  a(n) = Fibonacci(6*n)/8.
 * https://oeis.org/A049660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18·a(n−1) – a(n−2) a(0) = 0, a(1) = 1 (equivalently a(n)
 *            = Fibonacci(6n) / 8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* mutable cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    seq_t *tmp = NULL;  /* temporary pointer used after realloc */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* grow cache and compute missing values if requested n is larger */
    if (n >= cache_size) {
        tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        for (idx_t i = cache_size; i <= n; ++i) {
            tmp[i] = (seq_t)18 * tmp[i-1] - tmp[i-2];
        }
        cache = tmp;
        cache_size = n + 1;
    }

    return cache[n];
}
