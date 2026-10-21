/*
 * A087935  Perrin sequence of order 5.
 * https://oeis.org/A087935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5), with a(0)=5, a(1)=a(2)=a(3)=0, a(4)=4
 * Range:     u8 n <= 35, u16 n <= 71, u32 n <= 143, u64 n <= 286,
 *            u128 n <= 573, big any n
 * Helpers:   alloc_seq_array, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    if (cache == NULL) {
        cache = alloc_seq_array(5);
        cache_cap = 5;
        cache[0] = 5;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 4;
        last_computed = 4;
    }

    if (n <= 4) {
        return cache[n];
    }

    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-4] + cache[i-5];
    }

    last_computed = n;
    return cache[n];
}
