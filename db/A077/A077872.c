/*
 * A077872  Expansion of 1 / ((1-x)*(1-x+x^2+x^3)).
 * https://oeis.org/A077872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 33, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + a(n-4)  for n > 3
 * Initial values: a(0)=1, a(1)=2, a(2)=2, a(3)=0
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)0;
        cache_len = 4;
    }

    if (n < cache_len) {
        return cache[n];
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            if (new_cap < (size_t)(i + 1)) new_cap = i + 1;
            seq_t* tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        if (i >= 4) {
            cache[i] = 2 * cache[i-1] - 2 * cache[i-2] + cache[i-4];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
