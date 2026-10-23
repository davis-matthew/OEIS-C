/*
 * A047946  a(n) = 5*F(n)^2 + 3*(-1)^n where F(n) are the Fibonacci numbers
 *          A000045.
 * https://oeis.org/A047946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-a(n-3); a(0)=3, a(1)=2, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 3;
        cache = seq_alloc(cache_cap);          /* allocate array of seq_t */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache_len = 3;
    }

    /* if requested value is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    if ((idx_t)(cache_cap) < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : n + 1;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-2) - a(i-3) */
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        cache[i] = v1 * (seq_t)2 + v2 * (seq_t)2 - v3;
    }

    cache_len = n + 1;
    return cache[n];
}
