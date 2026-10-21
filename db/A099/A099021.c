/*
 * A099021  Main diagonal of array in A099020.
 * https://oeis.org/A099021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-3)*a(n-1) - (n-1)*(4*n-7)*a(n-2) -
 *            (n-2)*(n-1)*a(n-3) with initial values: a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored (>=3 after init) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache exists and contains the first three terms */
    if (cache_len == 0) {
        cache_cap = (n + 1 > 3) ? (n + 1) : 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 as a sentinel */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    /* If requested index already computed, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow cache if necessary */
    if (cache_len <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = (seq_t)(4*i - 3) * cache[i-1];
        seq_t t2 = (seq_t)(i-1) * (seq_t)(4*i - 7) * cache[i-2];
        seq_t t3 = (seq_t)(i-2) * (seq_t)(i-1) * cache[i-3];
        cache[i] = t1 - t2 - t3;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
