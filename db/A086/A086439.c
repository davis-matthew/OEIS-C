/*
 * A086439  Probability of obtaining a run of n consecutive cards of the same
 *          color in a deck of 52, numerators.
 * https://oeis.org/A086439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, frac
 * Formula:   a(n) = numerator of the fraction that equals the probability
 *            that a random 52‑card deck (26 red, 26 black) contains at least
 *            one run of n consecutive cards of the same color. In other
 *            words, if P_n = A(n)/B(n) is that probability written in lowest
 *            terms, then a(n) = A(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n = 1 => probability 1, numerator 1. */
    if (n <= 1)
        return 1;

    /* n > 26 => impossible to have a run. */
    if (n > 26)
        return 0;

    const idx_t max_run = n - 1;          /* no run of length >= n allowed */

    /* DP table: dp[last_color][run_length][red_used] */
    static seq_t dp[2][27][27];
    static seq_t dp_next[2][27][27];
    int i, j, r;

    /* clear DP tables */
    memset(dp, 0, sizeof(dp));
    memset(dp_next, 0, sizeof(dp_next));

    /* first card: red or black */
    dp[0][1][1] = 1;   /* first card red */
    dp[1][1][0] = 1;   /* first card black */

    /* process remaining 51 cards */
    for (idx_t step = 1; step < 52; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (i = 0; i < 2; ++i) {
            for (j = 1; j <= (int)max_run; ++j) {
                for (r = 0; r <= 26; ++r) {
                    seq_t cur = dp[i][j][r];
                    if (!cur)
                        continue;

                    /* same color, extend run */
                    if (j + 1 <= (int)max_run) {
                        idx_t new_r = r + (i == 0);
                        if (new_r <= 26)
                            dp_next[i][j + 1][new_r] += cur;
                    }

                    /* opposite color, start new run */
                    int new_i = 1 - i;
                    idx_t new_r = r + (new_i == 0);
                    if (new_r <= 26)
                        dp_next[new_i][1][new_r] += cur;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* patterns that avoid a run of length >= n */
    seq_t avoid = 0;
    for (i = 0; i < 2; ++i)
        for (j = 1; j <= (int)max_run; ++j)
            avoid += dp[i][j][26];

    /* total number of color patterns: C(52,26) */
    seq_t total = 1;
    for (idx_t k = 1; k <= 26; ++k)
        total = (total * (52 + 1 - k)) / k;

    /* favorable patterns */
    seq_t fav = total - avoid;

    /* reduce fav/total to lowest terms and return the numerator */
    seq_t g = gcd(fav, total);
    return fav / g;
}
