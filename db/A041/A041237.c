/*
 * A041237  Denominators of continued fraction convergents to sqrt(130).
 * https://oeis.org/A041237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 114*a(n-3) + a(n-6) with a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=112, a(4)=229, a(5)=570
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize base values if not already done */
    if (cache_len == 0) {
        cache_len = 6; /* indices 0..5 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)112;
        cache[4] = (seq_t)229;
        cache[5] = (seq_t)570;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Grow cache to accommodate index n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1; /* need indices 0..n inclusive */
    if (new_len > (idx_t)SIZE_MAX / sizeof(seq_t)) {
        /* Too large for allocation, return 0 as fallback */
        return (seq_t)0;
    }
    size_t new_size = (size_t)new_len * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_size);
    if (!tmp) {
        /* reallocation failed, return 0 */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = old_len; i < new_len; ++i) {
        cache[i] = (seq_t)114 * cache[i - 3] + cache[i - 6];
    }
    cache_len = new_len;

    return cache[n];
}
