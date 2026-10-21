/*
 * A215492  a(n) = 21*a(n-2) + 7*a(n-3), with a(0)=0, a(1)=3, and a(2)=6.
 * https://oeis.org/A215492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 21*a(n-2) + 7*a(n-3); a(0)=0, a(1)=3, a(2)=6 */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

seq_t a(idx_t n) {
    /* grow cache to hold indices up to n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* initialize base values if necessary */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            if (new_size > 1) cache[1] = (seq_t)3;
            if (new_size > 2) cache[2] = (seq_t)6;
        } else {
            if (cache_size <= 1 && new_size > 1) cache[1] = (seq_t)3;
            if (cache_size <= 2 && new_size > 2) cache[2] = (seq_t)6;
        }

        /* compute missing terms */
        idx_t start = cache_size;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)21 * cache[i-2] + (seq_t)7 * cache[i-3];
        }
        cache_size = new_size;
    }
    return cache[n];
}
