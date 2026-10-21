/*
 * A051607  a(n) = (3*n+7)!!!/7!!!.
 * https://oeis.org/A051607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n+7)!!! / 7!!!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values a(0)…a(k) */
    static idx_t  cache_size = 0; /* number of cached terms */

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache       = (seq_t*)malloc(sizeof(seq_t));
        cache[0]    = (seq_t)1;   /* 7!!! = 28 cancels with the product */
        cache_size  = 1;
    }

    /* extend cache up to requested n */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();                     /* allocation failed */
        cache = tmp;
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t factor = (seq_t)k * (seq_t)3 + (seq_t)7; /* 3*k+7 */
            cache[k] = cache[k - 1] * factor;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
