/*
 * A047902  a(1) = 11; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 11; for n > 1, a(n) = a(n-1) * digit_sum(a(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Cache of computed sequence values.  cache[0] holds a(1). */
    static seq_t *cache   = NULL;
    static idx_t  cached_n = 0;   /* number of terms stored */
    static size_t cap      = 0;   /* allocated capacity of cache */

    if (n == 0) {
        /* Undefined for n <= 0; return 0 as a fallback. */
        return (seq_t)0;
    }

    /* Return from cache if already computed. */
    if (n <= cached_n) {
        return cache[n - 1];
    }

    /* Ensure the cache has enough space. */
    size_t needed = (size_t)n;
    if (needed > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return 0. */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the first term if the cache is empty. */
    if (cached_n == 0) {
        cache[0] = (seq_t)11;  /* a(1) = 11 */
        cached_n = 1;
    }

    /* Compute remaining terms up to n. */
    for (idx_t i = cached_n; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t s    = digit_sum(prev);
        cache[i]   = prev * s;
    }
    cached_n = n;

    return cache[n - 1];
}
