/*
 * A138569  First differences of A137776.
 * https://oeis.org/A138569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4a(n-2) - 6a(n-4) + 4a(n-6) for n > 7 Initial values:
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=1, a(7)=0
 * Range:     i8 n <= 19, i16 n <= 35, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    // static cache for computed terms
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   // number of terms stored
    static size_t cache_cap = 0;   // allocated capacity

    // small initial table (definitely needed to start the recurrence)
    static const seq_t init[8] = {0, 1, 0, 1, 0, 1, 1, 0};

    /* If the requested index is among the initial constants, return it immediately. */
    if (n < 8) {
        return init[(size_t)n];
    }

    /* Allocate the cache on the first call that needs more than 8 terms. */
    if (!cache) {
        cache_cap = 16;                 /* start with some spare capacity */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        cache_len = 8;
    }

    /* Grow the cache if necessary to reach the requested index. */
    if (n >= cache_len) {
        if (n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap <= (size_t)n) new_cap *= 2;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)4 * cache[i - 2]
                        - (seq_t)6 * cache[i - 4]
                        + (seq_t)4 * cache[i - 6];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
