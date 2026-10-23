/*
 * A199592  Generalized Fermat numbers: 11^(2^n) + 1, n >= 0.
 * https://oeis.org/A199592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1)-1)^2 + 1, a(0) = 12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms.
       cache_len is the number of terms stored (0 .. cache_len-1). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    size_t target = (size_t)n;

    /* Ensure the cache is large enough. */
    if (target >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        while (new_cap <= target) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to target. */
    if (target >= cache_len) {
        for (size_t i = cache_len; i <= target; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)12;
            } else {
                seq_t prev = cache[i-1];
                seq_t sub1 = prev - (seq_t)1;
                seq_t sq   = seq_mul(sub1, sub1);
                cache[i]   = sq + (seq_t)1;
            }
        }
        cache_len = target + 1;
    }

    return cache[n];
}
