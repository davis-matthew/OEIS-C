/*
 * A074523  a(n) = 1^n + 7^n + 8^n.
 * https://oeis.org/A074523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) + 42, a(0)=3, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored (next free index) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* extend cache up to index n */
    while (cache_len <= n) {
        if (cache_len == cache_cap) {               /* grow array */
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        if (cache_len == 1) {                        /* a(1) */
            cache[1] = (seq_t)16;
        } else {                                     /* recurrence */
            cache[cache_len] =
                (seq_t)15 * cache[cache_len - 1] -
                (seq_t)56 * cache[cache_len - 2] +
                (seq_t)42;
        }
        cache_len++;
    }
    return cache[n];
}
