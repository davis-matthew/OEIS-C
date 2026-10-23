/*
 * A165148  a(n) = (3*8^n-4^n)/2.
 * https://oeis.org/A165148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*8^n-4^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=10, a(n)=12*a(n-1)-32*a(n-2) for n>1 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure we have computed up to index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)10;
            else
                cache[i] = cache[i - 1] * (seq_t)12 - cache[i - 2] * (seq_t)32;
        }
        cache_len = new_len;
    }

    return cache[n];
}
