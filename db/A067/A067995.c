/*
 * A067995  Minimal number of coins needed to pay exactly n cents using coins
 *          of sizes 1, 2, 3, 5, 10, 20, 25, 50, 100 cents (all historical
 *          U.S.A. coinage from 1 to 100 cents).
 * https://oeis.org/A067995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coin change DP for denominations {1,2,3,5,10,20,25,50,100} */
#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* a(0) unused, safety */

    /* ensure table covers up to n */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 1);
        dp[0] = 0;
        dp_size = 1;
    }
    if (n >= dp_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* allocation failure; not expected */
        dp = tmp;
        static const idx_t coins[] = {1,2,3,5,10,20,25,50,100};
        const size_t coin_cnt = sizeof(coins) / sizeof(coins[0]);
        for (idx_t i = dp_size; i <= n; ++i) {
            seq_t best = i; /* worst case: i coins of 1 cent */
            for (size_t c = 0; c < coin_cnt; ++c) {
                idx_t coin = coins[c];
                if (coin > i) break;
                seq_t cand = dp[i - coin] + 1;
                if (cand < best) best = cand;
            }
            dp[i] = best;
        }
        dp_size = new_size;
    }
    return dp[n];
}
