/*
 * A009233  Expansion of e.g.f. exp(sinh(x)*x) (even powers only).
 * https://oeis.org/A009233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1..n} binomial(2 n-1, 2 k-1) * k * a(n-k), a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial_internal.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* allocate the cache on the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – return 0 */
        cache[0] = (seq_t)1;                  /* a(0)  */
        cache_cap = 1;
    }

    /* grow the cache if necessary */
    if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = (size_t)(n) + 1;
        if (new_cap == 0 || new_cap > SIZE_MAX) return (seq_t)0;  /* overflow guard   */
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) return (seq_t)0;          /* reallocation failure – return 0 */
        cache = new_mem;

        /* compute all missing terms */
        for (idx_t i = cache_cap; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = (idx_t)1; k <= i; ++k) {
                seq_t coeff = binomial_internal((idx_t)(2 * i - 1),
                                                (idx_t)(2 * k - 1));
                seq_t term  = coeff * (seq_t)k * cache[i - k];
                sum += term;
            }
            cache[i] = sum * (seq_t)2;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
