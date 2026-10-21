/*
 * A086440  Probability of obtaining a run of n consecutive cards of the same
 *          color in a deck of 52, denominators.
 * https://oeis.org/A086440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, fini, full
 * Formula:   a(n) = reduced denominator of the probability that a random *
 *            arrangement of 52 cards containing 26 red and 26 black cards *
 *            contains a run of n consecutive cards of the same color
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd, total_combination
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/total_combination.h"

/* DP arrays (max indices 26 for counts, 26 for run length, 2 for color). */
static seq_t memo_dp[27][27][27][2];
static bool vis_dp[27][27][27][2];

/* Recursive DP for counting binary strings of length 52 with 26 zeros
 * and 26 ones, avoiding any run of length >= n. Assumes n >= 2. */
static seq_t dfs_dp(int zeros, int ones, int runlen, int col, int n) {
    if (zeros == 0 && ones == 0) return 1;
    if (vis_dp[zeros][ones][runlen][col]) return memo_dp[zeros][ones][runlen][col];
    seq_t ans = 0;
    if (col == 0) {               /* last card was zero */
        if (zeros > 0 && runlen + 1 < n)
            ans += dfs_dp(zeros - 1, ones, runlen + 1, 0, n);
        if (ones > 0)
            ans += dfs_dp(zeros, ones - 1, 1, 1, n);
    } else {                       /* last card was one */
        if (ones > 0 && runlen + 1 < n)
            ans += dfs_dp(zeros, ones - 1, runlen + 1, 1, n);
        if (zeros > 0)
            ans += dfs_dp(zeros - 1, ones, 1, 0, n);
    }
    vis_dp[zeros][ones][runlen][col] = true;
    memo_dp[zeros][ones][runlen][col] = ans;
    return ans;
}

/* Count sequences with no run of length >= n (n >= 2 and <= 26). */
static seq_t count_no_run(int n) {
    /* clear DP tables for the current n */
    for (int z = 0; z <= 26; ++z)
        for (int o = 0; o <= 26; ++o)
            for (int r = 1; r <= n - 1; ++r)
                for (int c = 0; c < 2; ++c)
                    vis_dp[z][o][r][c] = false;

    seq_t res = 0;
    /* first card could be zero or one */
    if (26 > 0) res += dfs_dp(25, 26, 1, 0, n);   /* start with zero */
    if (26 > 0) res += dfs_dp(26, 25, 1, 1, n);   /* start with one */
    return res;
}

/* Cache computed values of L(n) for n = 1..26 */
static seq_t L_cache[27];
static bool L_comp[27] = {false};

seq_t a(idx_t n) {
    int m = (int)n;
    if (m > 26) return 1;                 /* no run possible, probability 1 */
    if (!L_comp[m]) {
        if (m == 1) L_cache[m] = 0;       /* no sequences avoid a run of 1 */
        else L_cache[m] = count_no_run(m);
        L_comp[m] = true;
    }
    seq_t total = total_combination();
    seq_t L = L_cache[m];
    seq_t numer = total - L;
    seq_t g = gcd(numer, total);
    return total / g;
}
