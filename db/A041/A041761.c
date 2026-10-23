/*
 * A041761  Denominators of continued fraction convergents to sqrt(401).
 * https://oeis.org/A041761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 40*a(n-1) + a(n-2), a(0)=1, a(1)=40
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache to hold at least n+1 elements */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        abort();   /* allocation failure */
    }
    cache = new_cache;

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        if (new_len > 1) {
            cache[1] = (seq_t)40;
            cache_len = 2;
        }
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)40 * cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;
    return cache[n];
}
