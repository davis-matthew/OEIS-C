/*
 * A128128  Expansion of chi(-q^3) / chi^3(-q) in powers of q where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A128128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in χ(-q^3)/χ^3(-q) = ∏_{k≥1} (1 -
 *            q^k)^(-b_k), where b_k = 3 for k ≡ 1 or 5 (mod 6), b_k = 2 for k
 *            ≡ 3 (mod 6), otherwise 0
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 84, u64 n <= 289,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the required table by dynamic programming.
   For each k with b_k > 0 we perform b_k independent passes of the
   standard unbounded‑coin change DP, which incorporates the needed binomial
   coefficients automatically. */

#include <stdlib.h>

/* static cache for the DP table */
static seq_t *dp = NULL;          /* dp[n] == a(n) for 0 ≤ n ≤ current_max */
static size_t current_max = 0;    /* largest n for which dp is valid */

/* recompute the table from scratch up to and including N (N is size_t) */
static void recompute_dp(size_t N)
{
    /* allocate new table */
    seq_t *new_dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!new_dp) exit(1);          /* allocation failed */

    /* initialise all entries to 0 and set dp[0] = 1 */
    for (size_t i = 0; i <= N; ++i) new_dp[i] = (seq_t)0;
    new_dp[0] = (seq_t)1;

    /* apply the product factors for each k */
    for (size_t k = 1; k <= N; ++k) {
        int b = 0;
        switch (k % 6) {
            case 1: case 5:  b = 3; break;
            case 3:          b = 2; break;
            default:         b = 0; break;
        }
        if (!b) continue;

        /* perform b independent unbounded coin–change passes */
        for (int t = 0; t < b; ++t) {
            for (size_t idx = k; idx <= N; ++idx) {
                new_dp[idx] = new_dp[idx] + new_dp[idx - k];
            }
        }
    }

    /* free old table and store the new one */
    free(dp);
    dp = new_dp;
    current_max = N;
}

/* external interface required by the build */
seq_t a(idx_t n)
{
    /* convert index to size_t, safe for the intended range (≤ 10 000) */
    size_t N = (size_t)n;

    if (!dp || N > current_max)
        recompute_dp(N);

    return dp[N];
}
