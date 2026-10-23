/*
 * A049016  Expansion of 1/((1-x)^5 - x^5).
 * https://oeis.org/A049016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + 2*a(n-5)
 *            with initial values a(0)=1, a(1)=5, a(2)=15, a(3)=35, a(4)=70
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* small initial terms */
    static const seq_t init[5] = {1, 5, 15, 35, 70};

    /* Cached values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of stored terms */
    static size_t cache_cap = 0; /* allocated capacity */

    if (n < 5) {
        return init[(idx_t)n];
    }

    /* allocate / grow cache if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while ((size_t)n + 1 > new_cap) {
            new_cap <<= 1;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Break recurrence into two sums to avoid underflow */
        seq_t pos = 5 * cache[i - 1] + 10 * cache[i - 3] + 2 * cache[i - 5];
        seq_t neg = 10 * cache[i - 2] + 5 * cache[i - 4];
        cache[i] = pos - neg;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
