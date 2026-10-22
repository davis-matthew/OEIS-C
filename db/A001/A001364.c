/*
 * A001364  Number of ways of making change for n cents using coins of 1, 2,
 *          4, 12, 24, 48, 96, 120 cents (based on English coinage of 1939).
 * https://oeis.org/A001364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.:
 *            1/((1-x)*(1-x^2)*(1-x^4)*(1-x^12)*(1-x^24)*(1-x^48)*(1-x^96)*(1-x^120));
 *            computed by the coin-change DP over parts 1,2,4,12,24,48,96,120
 * Range:     u8 n <= 41, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"

static seq_t *a001364_tab = 0;
static idx_t a001364_len = 0;

seq_t a(idx_t n) {
    if (n >= a001364_len) {
        static const idx_t coins[8] = {1, 2, 4, 12, 24, 48, 96, 120};
        idx_t len = n + 1;
        if (len < 2 * a001364_len) len = 2 * a001364_len;
        if (len < 256) len = 256;
        seq_t *dp = alloc_zero(len);
        int i;
        idx_t m;
        dp[0] = 1;
        for (i = 0; i < 8; i++) {
            idx_t c = coins[i];
            for (m = c; m < len; m++) {
                dp[m] += dp[m - c];
            }
        }
        a001364_tab = dp;
        a001364_len = len;
    }
    return a001364_tab[n];
}
