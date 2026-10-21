/*
 * A077871  Expansion of (1-x)^(-1)/(1-x+x^2-2*x^3).
 * https://oeis.org/A077871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 2*a(n-4), with initial
 *            values a(0)=1, a(1)=2, a(2)=2, a(3)=3, derived from the
 *            generating function (1-x)^(-1)/(1-x+x^2-2*x^3)
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache holds a(0) … a(cache_cap-1) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;

    /* grow the cache if we have not reached the desired index yet */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;

        /* compute the missing terms */
        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                seq_t t1 = 2 * cache[i-1];
                seq_t t2 = 2 * (i >= 2 ? cache[i-2] : (seq_t)0);
                seq_t t3 = 3 * (i >= 3 ? cache[i-3] : (seq_t)0);
                seq_t t4 = 2 * (i >= 4 ? cache[i-4] : (seq_t)0);
                cache[i] = t1 - t2 + t3 - t4;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
