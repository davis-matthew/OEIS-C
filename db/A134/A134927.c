/*
 * A134927  a(0)=a(1)=1; a(n) = 3*(a(n-1) + a(n-2)).
 * https://oeis.org/A134927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 1; a(n) = 3 * (a(n-1) + a(n-2)) for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first two terms if needed */
    if (cache == NULL) {
        cache_len = 2;
        cache = mem_new(seq_t, cache_len);
        cache[0] = 1;
        cache[1] = 1;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Expand cache up to index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = mem_resize(seq_t, cache, new_len);
        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            cache[i] = sum * 3;
        }
        cache_len = new_len;
    }

    return cache[n];
}
