/*
 * A215785  Number of permutations of 0..floor((n*7-1)/2) on even squares of
 *          an n X 7 array such that each row, column, diagonal and
 *          (downwards) antidiagonal of even squares is increasing.
 * https://oeis.org/A215785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*a(n-2) - 99*a(n-4) - 2*a(n-6); Initial values: a(1)=1,
 *            a(2)=5, a(3)=42, a(4)=262, a(5)=2465, a(6)=15485
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; index 0 is unused */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;    /* highest index with known value */
    static size_t cache_cap = 0;   /* allocated array size (>= cache_max+1) */

    /* lazy initialization: create cache and store base values */
    if (!cache) {
        cache_cap = 8;                     /* capacity for indices 0..7 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;              /* allocation failure */
        cache[0] = 0;  /* dummy */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)262;
        cache[5] = (seq_t)2465;
        cache[6] = (seq_t)15485;
        cache_max = 6;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Ensure the cache array is large enough for index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i < 7) {            /* values 1..6 are already stored */
            continue;
        }
        cache[i] = cache[i-2] * 61
                 - cache[i-4] * 99
                 - cache[i-6] * 2;
    }
    cache_max = n;
    return cache[n];
}
