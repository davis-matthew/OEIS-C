/*
 * A221894  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor,
 *          without move-in move-out straight through or left turns.
 * https://oeis.org/A221894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 26*a(n-2) + 45*a(n-3) + 34*a(n-4) - 2*a(n-5) -
 *            4*a(n-6) + a(n-7) (n ≥ 8) Initial values: a(1) = 3 a(2) = 31
 *            a(3) = 266 a(4) = 2305 a(5) = 19936 a(6) = 172624 a(7) = 1494152
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache array */

    /* initialise cache with first 7 terms if not yet done */
    if (!cache) {
        cache_cap = 16;                       /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* out of memory */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)266;
        cache[3] = (seq_t)2305;
        cache[4] = (seq_t)19936;
        cache[5] = (seq_t)172624;
        cache[6] = (seq_t)1494152;
        cache_len = 7;
    }

    /* ensure the cache is large enough for n */
    size_t target = (size_t)n;          /* n is 1‑based index */
    if (target > cache_cap) {
        while (cache_cap < target)
            cache_cap <<= 1;            /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        cache = tmp;
    }

    /* compute missing terms up to n */
    if (target > cache_len) {
        for (size_t i = cache_len; i < target; ++i) {
            /* recurrence applies for i >= 7 (i is 0‑based index) */
            seq_t pos = 5 * cache[i - 1];
            pos += 26 * cache[i - 2];
            pos += 45 * cache[i - 3];
            pos += 34 * cache[i - 4];
            pos += cache[i - 7];        /* a(n-7) */

            seq_t neg = 2 * cache[i - 5];
            neg += 4 * cache[i - 6];

            cache[i] = pos - neg;
        }
        cache_len = target;
    }

    return cache[n - 1];
}
