/*
 * A009306  Expansion of e.g.f.: log(1 + exp(x)*x).
 * https://oeis.org/A009306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n – Σ_{k=1}^{n-1} C(n-1,k-1)·(n-k)·a(k) Initial values:
 *            a(0)=0, a(1)=1 (Recurrence from OEIS A009306)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;   /* array a[0 … cache_cap-1] */
static size_t cache_cap = 0;  /* number of allocated cells */

/*
 * Compute a(n) with memoisation.
 *  (seq_t and idx_t are provided by the build system.)
 */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Fill missing terms */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = 0;
                continue;
            }
            if (i == 1) {
                cache[i] = 1;
                continue;
            }
            seq_t sum  = 0;
            seq_t binom = 1;               /* C(i-1,0) */
            for (idx_t k = 1; k <= i - 1; ++k) {
                /* term = C(i-1,k-1) * (i-k) * a(k) */
                seq_t term = binom * (seq_t)(i - k) * cache[k];
                sum += term;
                if (k < i - 1) {
                    /* binom = C(i-1,k) = binom * (i-k) / k   */
                    binom = binom * (seq_t)(i - k) / (seq_t)k;
                }
            }
            cache[i] = (seq_t)i - sum;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
