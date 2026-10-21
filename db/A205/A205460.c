/*
 * A205460  Number of (n+1)X3 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock differing from each horizontal or
 *          vertical neighbor.
 * https://oeis.org/A205460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 0..3 arrays with the number of
 *            clockwise edge increases * in every 2×2 subblock differing from
 *            each horizontal or vertical neighbor. * Computation is performed
 *            by dynamic programming on rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* mapping of statistic pair (s0,s1) with s0!=s1 to state index 0..19 */
    static int8_t state_index[5][5];
    static int8_t s0vec[20], s1vec[20];
    static int8_t pair_idx[64][64];          /* pair_idx[a][b] = state index or -1 */
    static int8_t allow[20][20];             /* allow[s_prev][s_next] = 1 if
                                               vertical constraints satisfied      */
    static unsigned char digit[64][3];        /* precomputed digits of each row */
    static int initialized = 0;

    /* DP tables */
    static seq_t dp[64][20];
    static seq_t ndp[64][20];

    if (!initialized) {
        /* initialize state_index and vectors */
        int idx = 0;
        for (int s0 = 0; s0 <= 4; ++s0) {
            for (int s1 = 0; s1 <= 4; ++s1) {
                state_index[s0][s1] = -1;
            }
        }
        for (int s0 = 0; s0 <= 4; ++s0) {
            for (int s1 = 0; s1 <= 4; ++s1) {
                if (s0 != s1) {
                    state_index[s0][s1] = idx;
                    s0vec[idx] = (int8_t)s0;
                    s1vec[idx] = (int8_t)s1;
                    ++idx;
                }
            }
        }

        /* precompute digits for all 64 rows */
        for (int r = 0; r < 64; ++r) {
            digit[r][0] = r & 3;
            digit[r][1] = (r >> 2) & 3;
            digit[r][2] = (r >> 4) & 3;
        }

        /* helper for counting clockwise increases in a 2×2 block */
        for (int r0 = 0; r0 < 64; ++r0) {
            for (int r1 = 0; r1 < 64; ++r1) {
                /* compute s0 for columns 0-1 */
                int a0 = digit[r0][0], a1 = digit[r0][1];
                int b0 = digit[r1][0], b1 = digit[r1][1];
                int c0 = (a1 > a0) + (b1 > a1) + (b0 > b1) + (a0 > b0);

                /* compute s1 for columns 1-2 */
                int a2 = digit[r0][2], b2 = digit[r1][2];
                int c1 = (a2 > a1) + (b2 > a2) + (b1 > b2) + (a1 > b1);

                if (c0 == c1) {
                    pair_idx[r0][r1] = -1;
                } else {
                    pair_idx[r0][r1] = state_index[c0][c1];
                }
            }
        }

        /* vertical compatibility matrix */
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                allow[i][j] = (s0vec[i] != s0vec[j] && s1vec[i] != s1vec[j]) ? 1 : 0;
            }
        }

        initialized = 1;
    }

    /* DP initialization: first pair of rows (row0, row1) */
    memset(dp, 0, sizeof(dp));
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            int8_t id = pair_idx[r0][r1];
            if (id >= 0) {
                dp[r1][id] += (seq_t)1;
            }
        }
    }

    /* number of remaining transitions: pairs beyond the first one */
    int steps = (int)(n - 1);
    for (int step = 0; step < steps; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < 64; ++prev) {
            for (int s_prev = 0; s_prev < 20; ++s_prev) {
                seq_t val = dp[prev][s_prev];
                if (val == 0) continue;
                for (int cur = 0; cur < 64; ++cur) {
                    int8_t nxt = pair_idx[prev][cur];
                    if (nxt < 0) continue;
                    if (!allow[s_prev][nxt]) continue;
                    ndp[cur][nxt] += val;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* sum all final states */
    seq_t ans = 0;
    for (int r = 0; r < 64; ++r) {
        for (int s = 0; s < 20; ++s) {
            ans += dp[r][s];
        }
    }
    return ans;
}
