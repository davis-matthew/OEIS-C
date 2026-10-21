/*
 * A067357  Number of self-conjugate partitions of 4n+1 into odd parts.
 * https://oeis.org/A067357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of self-conjugate partitions of 4n+1 into odd
 *            parts. Implements the generating function G(q) = Σ_{k≥0}
 *            q^(k(k+1)) / ∏_{i=0}^{k} (1 – q^(2i+1)), by performing a
 *            coin‑change DP for the restricted odd parts
 * Range:     u8 n <= 43, u16 n <= 128, u32 n <= 418, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Cache previous results.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    size_t N = (size_t)n;
    if (N >= cache_cap) {
        /* Allocate cache large enough for indices 0..N. */
        size_t new_cap = N + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* memory allocation failed – return 0 */ return (seq_t)0; }
        cache = new_cache;

        /* Allocate temporary array for DP counting partitions into odd parts
           with an increasing bound on the largest odd part. */
        seq_t *dp = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!dp) { /* memory allocation failed – return 0 */ return (seq_t)0; }

        /* Initially only part 1 is allowed.  */
        dp[0] = (seq_t)1;
        for (size_t i = 1; i <= N; ++i)
            dp[i] = dp[i - 1];          /* partitions using only 1’s */

        /* Initialise answer array to zero. */
        for (size_t i = 0; i <= N; ++i)
            cache[i] = (seq_t)0;

        /* Process each k.  */
        for (size_t k = 0;; ++k) {
            size_t shift = k * (k + 1);
            if (shift > N) break;

            /* Add contribution of this k to all indices n ≥ shift. */
            for (size_t i = shift; i <= N; ++i)
                cache[i] += dp[i - shift];

            /* Add the next odd part for the following k (unless we've reached
               the maximum needed).  */
            if (shift == N) break;
            size_t p = 2 * k + 3;          /* next odd part */
            for (size_t i = p; i <= N; ++i)
                dp[i] += dp[i - p];
        }

        free(dp);
        cache_cap = new_cap;
    }
    return cache[n];
}
