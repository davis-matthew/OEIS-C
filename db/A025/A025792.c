/*
 * A025792  Expansion of 1/((1-x)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A025792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A025792: number of partitions of n into parts 1, 9, and 11.
 * Computed via a classic coin‑change dynamic–programming table.
 *
 * a(n) = |{(a,b,c) ∈ ℕ³ : a + 9b + 11c = n}|.
 */
#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;   /* dp[i] = a(i) */
static idx_t dp_size = 0;  /* current allocated size */

/* Ensure dp array is large enough for indices up to n and recompute it. */
static void recompute_dp(idx_t n)
{
    /* Allocate or resize the table to hold 0..n. */
    if (dp_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof *dp);
        if (!tmp) abort();           /* should not happen; halt on failure */
        dp = tmp;
        dp_size = n + 1;
    }

    /* Zero the entire table and set the base case. */
    memset(dp, 0, dp_size * sizeof *dp);
    dp[0] = 1;                      /* one way to partition zero */

    /* Coin denominations: 1, 9, 11. */
    const idx_t parts[3] = {1, 9, 11};

    /* Typical DP: for each coin, add its contributions for all sums. */
    for (int k = 0; k < 3; ++k) {
        const idx_t coin = parts[k];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }
}

/* Compute a(n).  Calls may come in any order. */
seq_t a(idx_t n)
{
    /* Recompute the DP table up to n each time; cost is O(n). */
    recompute_dp(n);
    return dp[n];
}
