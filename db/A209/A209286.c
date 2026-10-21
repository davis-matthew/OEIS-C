/*
 * A209286  a(n) = a(n-1) + (1+a(n-2))*a(n-3) for n>1, a(1) = 1, a(n) = 0 for
 *          n<1.
 * https://oeis.org/A209286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (1 + a(n-2))*a(n-3) for n>1; Initial values:
 *            a(0) = 0, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 13, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t needed = (size_t)n + 1;

    if (needed > cache_size) {
        /* Grow the cache and compute the new elements. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed, return zero */
        cache = tmp;

        for (size_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                /* a(2) = 1 by definition (since a(1)=1 and a(0)=0). */
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i-1] + ((seq_t)1 + cache[i-2]) * cache[i-3];
            }
        }
        cache_size = needed;
    }
    return cache[(size_t)n];
}
