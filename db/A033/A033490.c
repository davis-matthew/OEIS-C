/*
 * A033490  a(n) = 2*a(n-1) + a(floor(n/2)), with a(1) = 1, a(2) = 2.
 * https://oeis.org/A033490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, * a(n) = 2*a(n-1) + a(floor(n/2)), n ≥ 3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;

    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0, but harmless */

    /* Make sure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Compute values that have not yet been stored */
    if (computed < n) {
        if (computed == 0) {
            cache[1] = (seq_t)1;
            computed = 1;
            if (n >= 2) {
                cache[2] = (seq_t)2;
                computed = 2;
            }
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else {
                cache[i] = (seq_t)2 * cache[i - 1] + cache[i / 2];
            }
        }
        if (n > computed)
            computed = n;
    }

    return cache[n];
}
