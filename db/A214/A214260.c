/*
 * A214260  First differences of A052980.
 * https://oeis.org/A214260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(n+1)=2*a(n)+a(n-2)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    /* Initialise cache with first three terms */
    if (cache == NULL) {
        cached_size = 3;
        cache = (seq_t*)malloc(cached_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
    }

    /* Return already computed value */
    if (n < cached_size)
        return cache[n];

    /* Extend cache to size n+1 */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute new terms */
    for (idx_t i = cached_size; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + cache[i - 3];
    }
    cached_size = new_size;

    return cache[n];
}
