/*
 * A041213  Denominators of continued fraction convergents to sqrt(117).
 * https://oeis.org/A041213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1298*a(n-6) – a(n-12); initial terms: a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=11, a(4)=49, a(5)=60, a(6)=1249, a(7)=1309,
 *            a(8)=6485, a(9)=14279, a(10)=63601, a(11)=77880
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[12] = {
        1, 1, 5, 11, 49, 60, 1249, 1309, 6485, 14279, 63601, 77880
    };

    /* Handle small indices directly */
    if (n < 12)
        return init[(size_t)n];

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Allocate cache if first time */
    if (!cache) {
        cache_cap = 64;                 /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory: return 0 as sentinel */
        /* Copy the first 12 terms */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    /* Grow capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;             /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i)
            cache[i] = 1298 * cache[i - 6] - cache[i - 12];
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
