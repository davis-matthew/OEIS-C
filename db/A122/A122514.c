/*
 * A122514  Expansion of x/(1 - 2*x^2 - x^3 + x^4).
 * https://oeis.org/A122514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-4); a(0)=0, a(1)=1, a(n)=0 for
 *            n<0
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache so that future calls are O(1) after an initial pass */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t old_size = cache_size;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (!cache) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
            if (!cache) return (seq_t)0;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
            if (!tmp) return (seq_t)0;
            cache = tmp;
        }
        cache_size = new_size;
    }

    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t val = 0;
            val += (seq_t)2 * cache[i - 2];
            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val -= cache[i - 4];
            cache[i] = val;
        }
    }

    return cache[n];
}
