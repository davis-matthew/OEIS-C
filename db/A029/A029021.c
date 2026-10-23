/*
 * A029021  Expansion of 1/((1-x)*(1-x^2)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 7 and 9
 * Range:     u8 n <= 48, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of partition counts */
#ifdef __cplusplus
/* In C++ use new/delete; this ensures proper construction/destruction of
   non‑trivial seq_t objects. */
static seq_t *dp = nullptr;
static idx_t dp_size = 0;

static void ensure_capacity(idx_t n) {
    if (n < dp_size)
        return;                      /* already computed */

    /* allocate a fresh array of size n+1 */
    seq_t *new_dp = new seq_t[n + 1]; /* default constructor zeroes elements */

    /* initialise base case */
    new_dp[0] = (seq_t)1;

    /* parts allowed in the partitions */
    static const idx_t parts[4] = {1, 2, 7, 9};

    /* compute the DP table */
    for (idx_t p = 0; p < 4; ++p) {
        const idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            new_dp[i] += new_dp[i - coin];
        }
    }

    /* dispose of old cache and store the new one */
    delete[] dp;
    dp = new_dp;
    dp_size = n + 1;
}
#else
/* In C use realloc; seq_t is assumed to be trivially copyable. */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

static void ensure_capacity(idx_t n) {
    if (n < dp_size)
        return;                      /* already computed */

    /* grow the array with realloc */
    seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
    if (!new_dp)
        return;                      /* allocation failed – caller will see 0 */

    dp = new_dp;

    /* initialise all entries to 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* base case */
    dp[0] = (seq_t)1;

    /* parts allowed in the partitions */
    static const idx_t parts[4] = {1, 2, 7, 9};

    /* compute the DP table */
    for (idx_t p = 0; p < 4; ++p) {
        const idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    dp_size = n + 1;
}
#endif

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return dp[n];
}
