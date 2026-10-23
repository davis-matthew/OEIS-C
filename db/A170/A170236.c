/*
 * A170236  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ n < 40: a(n) = 33 * (a(0)+a(1)+…+a(n-1)) + 2
 *            for n ≥ 40: a(n) = 33 * (a(n-39)+a(n-38)+…+a(n-1)) - 561 *
 *            a(n-40)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a memoised array to store previously computed
   terms.  It works for all builds (fixed‑size integer types or the
   arbitrary‑precision class) as long as `seq_t` supports the arithmetic
   operators and default construction. */

#include <stdlib.h>          /* for malloc, realloc */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* memoised terms */
    static idx_t  cache_len = 0;  /* number of stored terms */

    /* Return value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to at least n+1 elements */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed: return zero as a safe fallback */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;                       /* a(0) = 1 */
        } else if (i < 40) {
            /* sum of all previous terms */
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];
            val = sum * 33 + 2;           /* a(i) = 33*sum + 2 */
        } else {
            /* sum of the previous 39 terms */
            seq_t sum = 0;
            for (idx_t j = i - 39; j < i; ++j)
                sum += cache[j];
            val = sum * 33 - (cache[i - 40] * 561); /* a(i) = 33*sum – 561*a(i-40) */
        }
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
