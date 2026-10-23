/*
 * A017847  Expansion of 1/(1 - x^6 - x^7).
 * https://oeis.org/A017847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + a(n-7) for n > 6 Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1
 * Range:     u8 n <= 69, u16 n <= 121, u32 n <= 225, u64 n <= 432,
 *            u128 n <= 847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (!cache) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)1;
        cache_len = 7;
    }

    if (n < cache_len) {
        return cache[n];
    }

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* reallocation failed */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 6] + cache[i - 7];
    }
    cache_len = new_size;

    return cache[(idx_t)n];
}
