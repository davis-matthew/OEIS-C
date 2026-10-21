/*
 * A086438  Probability of obtaining a run of n consecutive cards of the same
 *          color in a deck of 52, normalized by binomial(52,26).
 * https://oeis.org/A086438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = binom(52,26) – f(n–1) where f(m) is the number of 52‑card
 *            decks with 26 red and 26 black cards that have no run of more
 *            than m consecutive cards of the same color. The function
 *            computes f(m) by dynamic programming for all m ≤ 26 and returns
 *            a(n) for any n ≥ 1. For n > 26 the probability is 0
 * Range:     u8 none, u16 none, u32 none, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* maximum allowed run length (≤ 26) */
#define MAX_M 26

/* DP storage: dp[i][j][c][t]  i zeros, j ones, last colour c, run length t 1..MAX_M */
static seq_t dp[27][27][2][27];
static seq_t f[MAX_M+1];          /* f[m] for m = 0..26 */
static seq_t total_val;           /* binom(52,26) */
static bool initialized = false;

/* initialise total and f[0..26] */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* total number of arrangements of 26 red and 26 black cards */
    total_val = binom(52, 26);

    /* f[0] = 0  (no arrangement can have max run length 0) */
    f[0] = 0;

    /* compute f[m] for m = 1..26 */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        /* clear DP table for this m */
        for (idx_t i = 0; i <= 26; ++i) {
            for (idx_t j = 0; j <= 26; ++j) {
                for (int c = 0; c < 2; ++c) {
                    for (idx_t t = 1; t <= m; ++t) {
                        dp[i][j][c][t] = 0;
                    }
                }
            }
        }

        /* base cases: first card */
        dp[1][0][0][1] = 1;   /* start with a zero (red) */
        dp[0][1][1][1] = 1;   /* start with a one (black) */

        /* DP over total length l = i + j from 1 to 51 */
        for (idx_t l = 1; l <= 51; ++l) {
            for (idx_t i = 0; i <= 26 && i <= l; ++i) {
                idx_t j = l - i;
                if (j > 26) continue;

                for (int c = 0; c < 2; ++c) {
                    for (idx_t t = 1; t <= m; ++t) {
                        seq_t cur = dp[i][j][c][t];
                        if (cur == 0) continue;

                        /* add a zero (red) */
                        if (i < 26) {
                            if (c == 0) { /* same color, extend run */
                                if (t + 1 <= m)
                                    dp[i + 1][j][0][t + 1] += cur;
                            } else { /* switch color, start new run */
                                dp[i + 1][j][0][1] += cur;
                            }
                        }

                        /* add a one (black) */
                        if (j < 26) {
                            if (c == 1) { /* same color, extend run */
                                if (t + 1 <= m)
                                    dp[i][j + 1][1][t + 1] += cur;
                            } else { /* switch color, start new run */
                                dp[i][j + 1][1][1] += cur;
                            }
                        }
                    }
                }
            }
        }

        /* sum all ways that consume 26 reds and 26 blacks */
        seq_t sum = 0;
        for (int c = 0; c < 2; ++c) {
            for (idx_t t = 1; t <= m; ++t) {
                sum += dp[26][26][c][t];
            }
        }
        f[m] = sum;
    }
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    init();

    if (n == 0) return 0;        /* undefined, return 0 */
    if (n <= 1) return total_val;/* a(1) = total number of decks */

    if (n > 26) return 0;        /* run of length > 26 impossible */

    idx_t m = n - 1;             /* we need number of decks with max run ≤ m */
    seq_t res = total_val - f[m];
    return res;
}
