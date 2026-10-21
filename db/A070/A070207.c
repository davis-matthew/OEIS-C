/*
 * A070207  Expansion of (1-x-5*x^2)/(1-3*x-2*x^2-x^3).
 * https://oeis.org/A070207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+2*a(n-2)+a(n-3) with a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    ensure_cache(n, &cache, &cache_cap);

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] + 2 * cache[i - 2] + cache[i - 3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
