/*
 * A015099  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-4.
 * https://oeis.org/A015099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = Sum_{i=0..n} q^i*a(i)*a(n-i) with q = -4 and a(0)=1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Ensure we have space for index 0 and store a(0)=1 */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache_max = 0;
        initialized = true;
    }

    /* If the requested index is already computed, just return it */
    if (n <= cache_max)
        return cache[n];

    /* Extend cache to accommodate up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute all remaining terms up to n */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        /* Compute a(cur) = Sum_{i=0..cur-1} q^i * a(i) * a(cur-1-i) */
        for (idx_t i = 0; i <= cur - 1; ++i) {
            /* q = -4 -> q^i = (-1)^i * 4^i = (-1)^i * (1 << (2*i)) */
            seq_t pow_q = (seq_t)1 << (2 * i);
            if (i & 1)          /* odd i gives negative sign */
                pow_q = -pow_q;

            seq_t prod = pow_q * cache[i] * cache[cur - 1 - i];
            sum += prod;
        }
        cache[cur] = sum;
        cache_max = cur;
    }

    return cache[n];
}
