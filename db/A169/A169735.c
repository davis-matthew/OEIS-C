/*
 * A169735  a(1) = 100; for n>1, a(n) = a(n-1) - digitsum(a(n-1)).
 * https://oeis.org/A169735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169735: a(1) = 100; for n>1, a(n) = a(n-1) - digitsum(a(n-1)) */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[0] holds a(1), cache[1] holds a(2), etc. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first term */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        cache[0] = (seq_t)100;            /* a(1) */
    }

    /* generate terms up to a(n) if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        /* realloc may return NULL under memory pressure; for brevity we ignore that case */
        cache = new_cache;
        for (idx_t i = cache_size; i < n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev - digit_sum(prev);
        }
        cache_size = n;
    }

    return cache[n - 1];
}
