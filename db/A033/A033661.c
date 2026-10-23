/*
 * A033661  Trajectory of 31 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 31; a(n) = a(n-1) + reverse_decimal(a(n-1))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache_cap = 1;
        cache[0] = (seq_t)31;
        cache_max = 0;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if (n > cache_max) {
        size_t target = (size_t)(n + 1);
        size_t new_cap = cache_cap;
        while (new_cap < target) new_cap *= 2;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute any missing terms. */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t rev = reverse_decimal(cache[i - 1]);
            cache[i] = cache[i - 1] + rev;
        }
        cache_max = n;
    }

    return cache[n];
}
