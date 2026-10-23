/*
 * A016290  Expansion of g.f. 1/((1-2*x)*(1-4*x)*(1-8*x)).
 * https://oeis.org/A016290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 14*a(n-1) - 56*a(n-2) + 64*a(n-3) for n >= 3, with
 *            initial values a(0)=1, a(1)=14, a(2)=140
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0; /* next free index */

    /* Ensure capacity for the requested index. */
    if (n >= cache_len) {
        if (cache_cap < n + 1) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < n + 1) new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialize base values if not yet present. */
        if (cache_len == 0) {
            cache[0] = 1;
            cache_len = 1;
        }
        if (cache_len <= 1 && n >= 1) {
            cache[1] = 14;
            cache_len = 2;
        }
        if (cache_len <= 2 && n >= 2) {
            cache[2] = 140;
            cache_len = 3;
        }

        /* Compute missing values up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = 14 * cache[i - 1];
            seq_t t2 = 56 * cache[i - 2];
            seq_t t3 = 64 * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
