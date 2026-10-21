/*
 * A067997  Number of (unordered) ways of making change for n cents using
 *          coins of 1/2, 1, 2, 3, 5, 10, 20, 25, 50, 100 cents (all
 *          historical U.S.A. coinage denominations up to 100 cents).
 * https://oeis.org/A067997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered ways to form n cents using the US
 *            coin denominations ½c,1c,2c,3c,5c,10c,20c,25c,50c,100c. The
 *            algorithm multiplies all coin values by 2 so that the smallest
 *            unit is a half‑cent, stores the denominations as integers, and
 *            uses one‑dimensional dynamic programming. The rout...
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coins in half‑cent units: 0.5c => 1, 1c => 2, 2c => 4, 3c => 6,
   5c =>10, 10c =>20, 20c =>40, 25c =>50, 50c =>100, 100c =>200  */
static const idx_t coin_vals[10] = {
    1,  2,  4,   6, 10, 20, 40, 50, 100, 200
};

seq_t a(idx_t n)
{
    /* Target value in half‑cent units */
    idx_t target = n * 2;

    /* Dynamically resize the DP table as needed */
    static seq_t *dp = NULL;
    static size_t dp_size = 0;

    if (target + 1 > dp_size) {
        seq_t *newdp = (seq_t*)realloc(dp, (target + 1) * sizeof(seq_t));
        if (!newdp) return 0;          /* allocation failed – return 0 */
        dp = newdp;
        dp_size = target + 1;
    }

    /* Clear the table before recomputing */
    for (idx_t i = 0; i <= target; ++i)
        dp[i] = 0;

    /* DP base case: one way to make value 0 */
    dp[0] = 1;

    /* Standard 1‑D coin change DP */
    for (int c = 0; c < 10; ++c) {
        idx_t v = coin_vals[c];
        for (idx_t i = v; i <= target; ++i)
            dp[i] += dp[i - v];
    }

    return dp[target];
}
