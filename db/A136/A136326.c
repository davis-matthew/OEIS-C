/*
 * A136326  a(n) = a(n-1) + 4a(n-2) - 4a(n-3).
 * https://oeis.org/A136326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3), with a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=9
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialisation */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache_len = 4;
    }

    /* quick return if already computed */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < (size_t)n + 1) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term = cache[i - 1];
        term += 4 * cache[i - 2];
        term -= 4 * cache[i - 3];
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
