/*
 * A139759  A Fibonacci-based recurrence.
 * https://oeis.org/A139759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1; a(n)=a(n-1)+a(n-2)+gcd(n,a(n-1))-gcd(n,a(n-2))
 *            for n>=2
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* cache[0..cache_len-1] */
    static idx_t cache_len = 0;

    /* Initialise cache with the first two values */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* Return a precomputed value if available */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Expand the cache to include index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {
        return 0;   /* allocation failed */
    }
    cache = tmp;

    /* Compute values that are now missing */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[(size_t)(i - 1)];
        seq_t prev2 = cache[(size_t)(i - 2)];
        seq_t g1 = gcd((seq_t)i, prev1);
        seq_t g2 = gcd((seq_t)i, prev2);
        cache[(size_t)i] = prev1 + prev2 + g1 - g2;
    }
    cache_len = (idx_t)new_len;

    return cache[(size_t)n];
}
