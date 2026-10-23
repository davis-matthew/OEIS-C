/*
 * A126495  Number of base 27 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A126495 – Number of base‑27 n‑digit numbers with adjacent digits
    differing by at most 3.
    Computation: dynamic programming with 27 states.
    For n = 0, a(0) = 1 (empty string).
    For n ≥ 1, define dp[1][d] = 1 for all digits d∈{0,…,26}.
    Then dp[k+1][d] = Σ_{d' : |d'-d| ≤ 3} dp[k][d'].
    The answer is a(n) = Σ_{d=0}^{26} dp[n][d]. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Cache for all computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated array size in elements */
    static idx_t cache_max = 0;     /* highest index currently cached */

    /* DP arrays for one step */
    static seq_t dp[27];
    static seq_t ndp[27];
    static bool dp_ready = false;   /* true when dp holds counts for length cache_max */
    (void)dp; (void)ndp; (void)dp_ready; /* silence unused marker warnings */

    /* Initialize on first call */
    if (!cache) {
        cache_size = 2;      /* indices 0 and 1 at least */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – not expected in this context */

        cache[0] = (seq_t)1;   /* a(0) = 1 */

        /* Initialise dp for length 1 */
        for (int i = 0; i < 27; ++i) dp[i] = (seq_t)1;
        /* sum of dp[1] = 27 */
        seq_t total1 = (seq_t)27;
        cache[1] = total1;
        cache_max = 1;
        dp_ready = true;
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        size_t new_size = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Extend DP from current cache_max up to n */
    for (idx_t len = cache_max + 1; len <= n; ++len) {
        /* reset ndp */
        for (int i = 0; i < 27; ++i) ndp[i] = (seq_t)0;

        for (int d = 0; d < 27; ++d) {
            int low  = d > 3 ? d - 3 : 0;
            int high = (d + 3) < 27 ? d + 3 : 26;
            seq_t sum = (seq_t)0;
            for (int p = low; p <= high; ++p) {
                sum += dp[p];
            }
            ndp[d] = sum;
        }

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 27; ++i) total += ndp[i];
        cache[len] = total;

        /* prepare dp for next iteration */
        for (int i = 0; i < 27; ++i) dp[i] = ndp[i];
    }

    cache_max = n;
    return cache[n];
}
