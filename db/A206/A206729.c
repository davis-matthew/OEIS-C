/*
 * A206729  Number of nX3 0..2 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×3 arrays of digits 0..2 that avoid the
 *            pattern z‑1 z‑1 z in any row, column or NW→SE diagonal. Computed
 *            by dynamic programming over the rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 27 possible rows (3^3), represented in base‑3 */
    static bool good[27];
    static uint8_t col[27][3];
    static uint8_t trans_list[27 * 27][27];
    static uint8_t trans_count[27 * 27];
    static bool init = false;

    if (!init) {
        /* good[row] : true if the row itself contains no forbidden
           pattern z‑1 z‑1 z.  Also store its digits in col[row][c]. */
        for (int r = 0; r < 27; ++r) {
            int v = r;
            col[r][0] = v % 3;  v /= 3;
            col[r][1] = v % 3;  v /= 3;
            col[r][2] = v % 3;
            if (col[r][0] == col[r][1] && col[r][1] + 1 == col[r][2])
                good[r] = false;
            else
                good[r] = true;
        }

        /* Precompute all admissible transitions from (prev2,prev1)
           to new row k. */
        for (int i = 0; i < 27; ++i) {
            if (!good[i]) continue;
            for (int j = 0; j < 27; ++j) {
                if (!good[j]) continue;
                int idx = i * 27 + j;
                uint8_t cnt = 0;
                for (int k = 0; k < 27; ++k) {
                    if (!good[k]) continue;
                    bool ok = true;
                    for (int c = 0; c < 3; ++c) {
                        if (col[i][c] == col[j][c] &&
                            col[i][c] + 1 == col[k][c]) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;

                    /* NW→SE diagonal constraint: (i,0),(j,1),(k,2) */
                    if (col[i][0] == col[j][1] &&
                        col[i][0] + 1 == col[k][2]) {
                        continue;
                    }
                    trans_list[idx][cnt++] = (uint8_t)k;
                }
                trans_count[idx] = cnt;
            }
        }
        init = true;
    }

    /* ----- handle small n ----- */
    if (n == 1) {
        seq_t cnt = 0;
        for (int r = 0; r < 27; ++r)
            if (good[r]) ++cnt;
        return cnt;
    }

    static seq_t dp[27 * 27];
    static seq_t dp_next[27 * 27];

    /* initialize DP for the first two rows (n ≥ 2) */
    for (int s = 0; s < 27 * 27; ++s) dp[s] = 0;
    for (int i = 0; i < 27; ++i) if (good[i])
        for (int j = 0; j < 27; ++j) if (good[j])
            dp[i * 27 + j] = 1;

    /* if requested n==2, sum all pairs */
    if (n == 2) {
        seq_t ans = 0;
        for (int s = 0; s < 27 * 27; ++s) ans += dp[s];
        return ans;
    }

    /* DP for rows 3 .. n */
    for (idx_t r = 3; r <= n; ++r) {
        for (int s = 0; s < 27 * 27; ++s) dp_next[s] = 0;

        for (int s = 0; s < 27 * 27; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int j = s % 27;          /* last row index */
            uint8_t cnt = trans_count[s];
            for (int p = 0; p < cnt; ++p) {
                int k = trans_list[s][p];   /* new row index */
                int dest = j * 27 + k;      /* new state index */
                dp_next[dest] += val;
            }
        }
        for (int s = 0; s < 27 * 27; ++s) dp[s] = dp_next[s];
    }

    /* final answer: sum over all states */
    seq_t ans = 0;
    for (int s = 0; s < 27 * 27; ++s) ans += dp[s];
    return ans;
}
