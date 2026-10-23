/*
 * A036240  Number of 3-way interactions when 3 subsets of power set on {1..n}
 *          are chosen at random; number of Boolean functions of n variables
 *          and rank 3 from Post class F(8,inf).
 * https://oeis.org/A036240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (8^n-7^n-3*4^n+3*3^n+2*2^n-2)/6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* The recurrence derived from the generating function:
       a(1) = 0, a(2) = 0, a(3) = 12, a(4) = 200,
       a(5) = 2280, a(6) = 22420
       a(n) = 25*a(n-1)-241*a(n-2)+1135*a(n-3)-2734*a(n-4)+3160*a(n-5)-1344*a(n-6) */

    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return 0;   /* undefined for the sequence, but harmless */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 7;                    /* indices 0 .. 6 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;

        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)200;
        cache[5] = (seq_t)2280;
        cache[6] = (seq_t)22420;
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* extend cache to store up to index n */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;

    /* compute missing terms using the linear recurrence */
    idx_t start = (old_size > 6) ? old_size : 7;
    for (idx_t k = start; k <= n; ++k) {
        seq_t big  = 25 * cache[k - 1];
        big      += 1135 * cache[k - 3];
        big      += 3160 * cache[k - 5];

        seq_t small = 241 * cache[k - 2];
        small     += 2734 * cache[k - 4];
        small     += 1344 * cache[k - 6];

        cache[k] = big - small;   /* guaranteed non‑negative */
    }

    return cache[n];
}
