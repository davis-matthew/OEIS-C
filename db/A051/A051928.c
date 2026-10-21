/*
 * A051928  Number of independent sets of vertices in graph K_3 X C_n (n > 2).
 * https://oeis.org/A051928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + a(n-3) initial values: a(0)=4,
 *            a(1)=1, a(2)=13
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* grow the cache to hold indices up to n */
    if (cache_len <= (size_t)n) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp;

        if (cache_len == 0) {
            tmp = (seq_t*) malloc(newlen * sizeof(seq_t));
        } else {
            tmp = (seq_t*) realloc(cache, newlen * sizeof(seq_t));
        }
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;

        /* compute values from the end of the previous cache
           up to the new length */
        for (size_t i = cache_len; i < newlen; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)4;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)13;
            } else {
                cache[i] = cache[i-1] * (seq_t)2
                         + cache[i-2] * (seq_t)4
                         + cache[i-3];
            }
        }
        cache_len = newlen;
    }

    return cache[(size_t)n];
}
