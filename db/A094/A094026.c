/*
 * A094026  Expansion of x(1+10x)/((1-x^2)(1-10x^2)).
 * https://oeis.org/A094026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(n) = a(n-1) + 1 if n is odd,
          a(n-1) * 10 if n is even. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* extend cache until it contains a(n) */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2 + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        if (cache_len == 1) {
            cache[1] = (seq_t)1;
        } else {
            idx_t i = cache_len;
            if ((i & 1) == 0)          /* even index */
                cache[i] = (seq_t)(cache[i-1] * (seq_t)10);
            else                        /* odd index  */
                cache[i] = (seq_t)(cache[i-1] + (seq_t)1);
        }
        ++cache_len;
    }

    return cache[n];
}
