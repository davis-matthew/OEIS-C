/*
 * A029292  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 722, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into parts 3, 6, 7, and 8
 * (OEIS A029292).  The values are obtained by standard coin‑change dynamic
 * programming:  a(0)=1 and for each part p ∈ {3,6,7,8} we iterate
 * a[i] += a[i−p] for all i ≥ p.
 */
#include <stdlib.h>

static seq_t *dp = NULL;           /* memoised table of a(n) values */
static idx_t  max_n = 0;           /* highest n for which dp[0..max_n] are valid */

seq_t a(idx_t n)
{
    /* If we have already computed up to this n, simply return the cached value */
    if (n < max_n) {
        return dp[n];
    }

    /* Need to recompute up to n (may be larger than any previous query) */
    /* Free old table if it exists */
    if (dp) {
        free(dp);
    }

    /* Allocate table for indices 0..n */
    size_t sz = (size_t)(n + 1);
    dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) {           /* out‑of‑memory; return 0 as a fallback */
        return 0;
    }

    /* Initialise all entries to 0 */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;          /* base case: one way to make sum 0 */

    /* The available parts */
    const idx_t parts[] = {3, 6, 7, 8};
    const size_t nparts = sizeof(parts) / sizeof(parts[0]);

    /* Dynamic programming: for each part, add its contributions */
    for (size_t p_idx = 0; p_idx < nparts; ++p_idx) {
        idx_t p = parts[p_idx];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Record that we have now computed up to n */
    max_n = n;

    return dp[n];
}
