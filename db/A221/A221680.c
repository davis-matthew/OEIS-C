/*
 * A221680  Number of 0..5 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..5 arrays of length n with each element
 *            differing from at least one neighbor by at most 1, starting with
 *            0. Computed via a DP over the last two elements; runs in O(n)
 *            time
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 0;

    static seq_t dp[6][6];
    static seq_t nxt[6][6];
    /* initialize dp to zero each call */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            dp[i][j] = 0;

    /* length 2 initialization: first element fixed at 0 */
    for (int a2 = 0; a2 <= 5; ++a2)
        if (a2 <= 1)          /* |a2-0| ≤ 1 */
            dp[0][a2] += 1;

    if (n == 2) {
        seq_t sum = 0;
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                sum += dp[i][j];
        return sum;
    }

    /* DP for positions 3 … n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                nxt[i][j] = 0;

        for (int prev2 = 0; prev2 <= 5; ++prev2) {
            for (int prev = 0; prev <= 5; ++prev) {
                seq_t cnt = dp[prev2][prev];
                if (cnt == 0) continue;

                int diff_common = (prev > prev2 ? prev - prev2 : prev2 - prev) <= 1;

                for (int cur = 0; cur <= 5; ++cur) {
                    int diff_right = (prev > cur ? prev - cur : cur - prev) <= 1;
                    if (!(diff_common || diff_right)) continue;
                    if (pos == n && !diff_right) continue; /* enforce last element's neighbor */
                    nxt[prev][cur] += cnt;
                }
            }
        }

        /* copy nxt into dp for next iteration */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = nxt[i][j];
    }

    seq_t total = 0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            total += dp[i][j];
    return total;
}
