/*
 * A238188  a(n) = 4*a(n-4) + 6*a(n-8) + 4*a(n-12) + a(n-16) for n>15, with
 *          the sixteen initial values as shown.
 * https://oeis.org/A238188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 55, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-4) + 6*a(n-8) + 4*a(n-12) + a(n-16) for n > 15,
 * with initial values (offset 0):
 * a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=2, a(7)=3,
 * a(8)=9, a(9)=11, a(10)=13, a(11)=15, a(12)=48, a(13)=57,
 * a(14)=68, a(15)=81.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;

    /* Initial values for n = 0..15 */
    static const seq_t init_vals[16] = {
        0, 0, 1, 1, 2, 2, 2, 3,
        9, 11, 13, 15, 48, 57, 68, 81
    };

    /* If cache is empty, allocate and store initial 16 terms */
    if (cache == NULL) {
        /* Allocate at least 16 + next power of 2 */
        cache_cap = 32; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure: return 0 */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cache_used = 16;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* realloc failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute up to n */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        if (i < 16) {
            /* already initialized */
            continue;
        }
        /* Recurrence */
        cache[i] = 4 * cache[i - 4] + 6 * cache[i - 8] +
                   4 * cache[i - 12] + cache[i - 16];
    }
    cache_used = (size_t)n + 1;

    return cache[(size_t)n];
}
