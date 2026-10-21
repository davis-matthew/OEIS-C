/*
 * A090158  Odd-indexed terms of the binomial transform equals 1 and the
 *          even-indexed terms of the second binomial transform equals 1.
 * https://oeis.org/A090158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(2n) = 1 - 2^(2n); a(2n+1) = 1 - Σ_{k=0}^{2n} C(2n+1,k)
 *            * a(k) (with C(n,k) binomial)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Static state to cache computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last = 0;          /* highest index computed */
    static seq_t pow2 = (seq_t)1;   /* 2^i; after first setup holds 2^0 */

    /* One‑time initialization */
    if (!cache) {
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        last = 0;
        pow2 = (seq_t)2;            /* so pow2 = 2^(1) for next index processed */
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        ensure_cache(n + 1, &cache, &cache_cap);
    }

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if ((i & 1u) == 0) {         /* even index */
            cache[i] = (seq_t)1 - pow2;
        } else {                     /* odd index */
            seq_t sum  = 0;
            seq_t binom = 1;          /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                sum += binom * cache[k];
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
            cache[i] = (seq_t)1 - sum;
        }
        /* Prepare pow2 for the next index (i+1): 2^(i+1) = 2^i * 2 */
        pow2 *= (seq_t)2;
        last = i;
    }

    return cache[n];
}
