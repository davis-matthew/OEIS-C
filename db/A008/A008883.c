/*
 * A008883  3x+1 sequence starting at 51.
 * https://oeis.org/A008883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Collatz iteration: a(0) = 51, a(n+1) = 3*a(n)+1 if a(n) odd, else a(n)/2 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* ensure capacity for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialize first term if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)51;
        cache_len = 1;
    }

    /* compute forward if needed */
    while (cache_len <= (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        if ((prev & (seq_t)1) == 0) {
            cache[cache_len] = prev >> 1;
        } else {
            cache[cache_len] = prev * (seq_t)3 + (seq_t)1;
        }
        cache_len++;
    }

    return cache[(size_t)n];
}
