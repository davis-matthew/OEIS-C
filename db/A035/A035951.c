/*
 * A035951  Number of partitions in parts not of the form 13k, 13k+3 or 13k-3.
 *          Also number of partitions with at most 2 parts of size 1 and
 *          differences between parts at distance 5 are greater than 1.
 * https://oeis.org/A035951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035951
   Number of partitions of n where every part is not congruent to 0, 3 or 10
   modulo 13 (i.e., not of the form 13k, 13k+3 or 13k+10).
   Computed by dynamic programming over the allowable part sizes.
   For each allowed part p, each dp[i] for i ≥ p is increased by dp[i‑p].
   The table is recomputed only when a larger n is requested.
   Complexity: O(max_n²) pre‑computation, O(1) per query once computed.
*/

#include <stdlib.h>

/* static state for memoisation */
static seq_t *dp     = NULL;   /* partition counts */
static size_t dp_cap = 0;      /* allocated capacity (0 … dp_cap-1) */
static idx_t  max_n  = 0;      /* largest n for which dp has been computed */

/* enlarge dp array if necessary */
static void ensure_capacity(idx_t n)
{
    if (dp_cap < (size_t)(n + 1)) {
        size_t new_cap = dp_cap ? dp_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();         /* allocation failure */
        dp = tmp;
        dp_cap = new_cap;
    }
}

/* recompute the entire DP table up to n (used only when a new larger n is requested) */
static void recompute(idx_t n)
{
    if (n <= max_n)      /* already computed */
        return;

    ensure_capacity(n);

    /* initialise: dp[0] = 1, others 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* dynamic programming over allowed part sizes */
    for (idx_t p = 1; p <= n; ++p) {
        if (p % 13u == 0u || p % 13u == 3u || p % 13u == 10u)
            continue;   /* disallowed part */

        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)                     /* sequence is offset 1; a(1) is first term */
        return 0;

    recompute(n);
    return dp[n];
}
