/*
 * A143460  Expansion of 1/(x^k*(1-x-3*x^(k+1))) for k=9.
 * https://oeis.org/A143460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-10) for n ≥ 10 Initial values: a(0)=1,
 *            a(1)=4, a(2)=7, a(3)=10, a(4)=13, a(5)=16, a(6)=19, a(7)=22,
 *            a(8)=25, a(9)=28
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 81, u64 n <= 167,
 *            u128 n <= 339, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure allocation of a given size */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first 10 terms on first call */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 7;
        cache[3] = 10;
        cache[4] = 13;
        cache[5] = 16;
        cache[6] = 19;
        cache[7] = 22;
        cache[8] = 25;
        cache[9] = 28;
        cache_len = 10;
    }

    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Compute terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i >= 10) {
            cache[i] = cache[i - 1] + 3 * cache[i - 10];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
