/*
 * A202155  x-values in the solution to x^2 - 13*y^2 = -1.
 * https://oeis.org/A202155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1298*a(n-1) - a(n-2); a(1) = 18; a(2) = 23382
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* up to n = 0 the function is not defined; guard anyway */
    if (n == 0)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    size_t need = (size_t)n;

    /* grow cache capacity if necessary */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    if (need > cache_len) {
        for (size_t i = cache_len; i < need; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)18;
            } else if (i == 1) {
                cache[i] = (seq_t)23382;
            } else {
                cache[i] = ((seq_t)1298) * cache[i - 1];
                cache[i] -= cache[i - 2];
            }
        }
        cache_len = need;
    }

    return cache[need - 1];
}
