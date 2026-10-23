/*
 * A105578  a(n+3) = 2a(n+2) - 3a(n+1) + 2a(n); a(0) = 1, a(1) = 1, a(2) = 0.
 * https://oeis.org/A105578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 33, i32 n <= 64, i64 n <= 129,
 *            i128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 2*a(n+2) - 3*a(n+1) + 2*a(n);
   Initial values: a(0)=1, a(1)=1, a(2)=0. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Grow the cache if needed */
    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            needed * sizeof(seq_t));
        assert(new_cache != NULL);          /* memory allocation success */
        cache = new_cache;
        for (size_t i = cache_len; i < needed; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 1;
            } else if (i == 2) {
                cache[i] = 0;
            } else {
                /* a(i) = 2*a(i-1) - 3*a(i-2) + 2*a(i-3) */
                cache[i] = 2 * cache[i - 1] - 3 * cache[i - 2] + 2 * cache[i - 3];
            }
        }
        cache_len = needed;
    }
    return cache[n];
}
