/*
 * A048498  a(n) = 2^(n-1)*(5*n-8) + 5.
 * https://oeis.org/A048498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3), a(0)=1, a(1)=2, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;

    ensure_cache(n, &cache, &cap);

    if (computed < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        computed = 2;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)5*cache[i-1] - (seq_t)8*cache[i-2] + (seq_t)4*cache[i-3];
        }
        computed = n;
    }

    return cache[n];
}
