/*
 * A054248  Binary entropy: a(n) = n + min { a(k)+a(n-k) : 1 <= k <= n-1 }.
 * https://oeis.org/A054248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + min{ a(k) + a(n-k) : 1 ≤ k ≤ n-1 } (OEIS A054248)
 * Range:     u8 n <= 45, u16 n <= 5266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Not defined for n == 0; return 0 to satisfy caller. */
    if (n == 0) return (seq_t)0;

    /* Cache to store computed values */
    static seq_t *cache = NULL;  /* cache[0] unused, cache[i] = a(i) */
    static idx_t  cache_size   = 0;   /* allocated length (elements) */
    static idx_t  last_computed = 0;  /* largest n for which cache is filled */

    /* Allocate or grow cache on first use or when n exceeds it. */
    if (cache == NULL) {
        cache_size = (n + 1 > 4) ? (n + 1) : 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = (seq_t)0;
    } else if (n + 1 > cache_size) {
        /* Grow cache to accommodate up to n.
           Doubling strategy keeps allocations amortised O(n). */
        idx_t new_size = cache_size;
        while (n + 1 > new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure: return 0 */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute new values up to n if necessary. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            /* Find minimal a(k)+a(i-k) over 1 ≤ k ≤ i-1. */
            seq_t best = cache[1] + cache[i-1];
            for (idx_t k = 2; k < i; ++k) {
                seq_t val = cache[k] + cache[i-k];
                if (val < best) best = val;
            }
            cache[i] = (seq_t)i + best;
        }
    }
    last_computed = n;
    return cache[n];
}
