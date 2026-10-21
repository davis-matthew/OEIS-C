/*
 * A077927  Expansion of (1-x)^(-1)/(1+2*x+2*x^3).
 * https://oeis.org/A077927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) - 2*a(n-3) + 2*a(n-4), with initial
 *            values a(0)=1, a(1)=-1, a(2)=3, a(3)=-7
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms */
    static idx_t cache_cap = 0;   /* capacity of the cache */

    /* extend cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;   /* grow factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)-1;
        else if (i == 2)     val = (seq_t)3;
        else if (i == 3)     val = (seq_t)-7;
        else
            val = -cache[i-1] + (seq_t)2 * cache[i-2] - (seq_t)2 * cache[i-3] + (seq_t)2 * cache[i-4];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
