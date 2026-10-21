/*
 * A075112  a(n)=Sum((-1)^(i+Floor(n/2))S(2i+e),(i=0,..,Floor(n/2))), where
 *          S(n) are generalized Tetranacci numbers (A073817) and
 *          e=(1/2)(1-(-1)^n).
 * https://oeis.org/A075112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)+2a(n-3)+2a(n-4)+a(n-5)+a(n-6), a(0)=4, a(1)=1,
 *            a(2)=-1, a(3)=6, a(4)=16, a(5)=20
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 33, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (>=0) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* grow the cache to hold up to index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= n) new_cap <<= 1;    /* double until large enough */
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first six values once */
    if (cache_len == 0) {
        for (idx_t i = 0; i < 6; ++i) {
            if (i == 0) cache[0] = (seq_t)  4;
            if (i == 1) cache[1] = (seq_t)  1;
            if (i == 2) cache[2] = (seq_t) -1;
            if (i == 3) cache[3] = (seq_t)  6;
            if (i == 4) cache[4] = (seq_t) 16;
            if (i == 5) cache[5] = (seq_t) 20;
        }
        cache_len = 6;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6)          /* base cases are already set */
            continue;
        /* recurrence: a(i) = a(i-1)+2a(i-3)+2a(i-4)+a(i-5)+a(i-6) */
        seq_t val = cache[i-1];
        val += cache[i-3] * (seq_t)2;
        val += cache[i-4] * (seq_t)2;
        val += cache[i-5];
        val += cache[i-6];
        cache[i] = val;
    }
    if (n + 1 > cache_len)
        cache_len = n + 1;

    return cache[n];
}
