/*
 * A052858  Expansion of e.g.f. log(-1/(-1+x*exp(x)-x)).
 * https://oeis.org/A052858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 0, a(1) = 0;
 * a(n) = n + Σ_{k=2}^{n-1} k * binomial(n-1,k) * a(n-k)
 * (where binomial(n,k) returns ⌊n k⌋ and a(n-k) is the already‑computed
 *   value of this sequence).  The function memoises results in a static
 *   cache so calls in any order are handled correctly.
 */

#include <stdlib.h>
#include <stdbool.h>            /* for bool */
#include "../utils/binomial.h"  /* binomial(idx_t, idx_t) → seq_t */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i < cache_len. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;   /* number of cached elements */

    /* Expand cache if necessary.  After expansion cache_len will be n+1. */
    if ((size_t)n + 1 > cache_len) {
        size_t old_len = cache_len;          /* keep old value for later */
        size_t new_len = (size_t)n + 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */

        /* Zero newly allocated part (which is also newly expanded). */
        for (size_t i = old_len; i < new_len; ++i)
            tmp[i] = 0;

        cache     = tmp;
        cache_len = new_len;

        /* Compute any missing values between old_len and n. */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i < 2) {
                cache[i] = 0;
                continue;
            }

            seq_t sum = 0;
            for (idx_t k = 2; k <= i - 1; ++k) {
                seq_t term = (seq_t)k * binomial(i - 1, k) * cache[i - k];
                sum += term;
            }
            cache[i] = (seq_t)i + sum;
        }
    }
    /* If the value is already computed, return it directly. */
    return cache[n];
}
