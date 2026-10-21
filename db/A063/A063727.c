/*
 * A063727  a(n) = 2*a(n-1) + 4*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A063727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 2. This implements
 *            an incremental memoized recurrence suitable for all supported
 *            seq_t sizes
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with first two terms. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* If requested value already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend cache up to n. */
    cache_size = n + 1;
    cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

    /* Compute missing terms sequentially. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Use seq_t arithmetic; constants cast to seq_t. */
        cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)4;
    }

    return cache[n];
}
