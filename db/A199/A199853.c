/*
 * A199853  Expansion of (1-3*x+x^3)/(1-2*x-x^2+x^3).
 * https://oeis.org/A199853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=-1, a(3)=-3, for n>=4:
 *            a(n)=2*a(n-1)+a(n-2)-a(n-3)
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 28, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n + 1 > cache_len) {
        /* Expand the cache */
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (old_len > 0)
            memcpy(new_cache, cache, old_len * sizeof(seq_t));

        if (cache)
            free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* Initialize base values that may not exist yet */
        if (cache_len >= 1) cache[0] = (seq_t)1;
        if (cache_len >= 2) cache[1] = (seq_t)-1;
        if (cache_len >= 3) cache[2] = (seq_t)-1;
        if (cache_len >= 4) cache[3] = (seq_t)-3;

        /* Compute any newly needed terms */
        for (idx_t i = (old_len > 4 ? old_len : 4); i < cache_len; ++i) {
            cache[i] = (seq_t)(
                (seq_t)2 * cache[i - 1] + cache[i - 2] - cache[i - 3]
            );
        }
    }
    return cache[n];
}
