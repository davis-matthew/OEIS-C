/*
 * A238175  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A238175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 4 arrays over {0,1,2} such that for
 *            every 2×2 subblock the sum of its minimum and maximum entry is
 *            the same for all subblocks. The function computes this quantity
 *            for n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2329, a(2) = 69023, … as in OEIS A238175 */
#include <stdbool.h>
#include <stdint.h>

#define N_ROWS 81  /* 3^4 possible rows */

seq_t a(idx_t n)
{
    /* Pre‑computed adjacency data for the five possible sums 0…4.  The data
       is constructed once on the first call and then reused. */
    static bool initialized = false;
    static unsigned char neigh_count[5][N_ROWS];
    static unsigned char neigh_list[5][N_ROWS][N_ROWS];
    static unsigned char adj_mat[5][N_ROWS][N_ROWS]; /* 0/1 flag, temporary */

    if (!initialized) {
        /* Pre‑compute the 4‑digit base‑3 representation of each row. */
        int row_vals[N_ROWS][4];
        for (int r = 0; r < N_ROWS; ++r) {
            int tmp = r;
            for (int k = 0; k < 4; ++k) {
                row_vals[r][k] = tmp % 3;
                tmp /= 3;
            }
        }

        /* Build adjacency matrices for each possible sum s (0…4). */
        for (int s = 0; s <= 4; ++s) {
            for (int a = 0; a < N_ROWS; ++a) {
                for (int b = 0; b < N_ROWS; ++b) {
                    int sum_val = -1;
                    bool ok = true;
                    for (int j = 0; j < 3 && ok; ++j) {
                        int mn = row_vals[a][j];
                        if (row_vals[a][j+1] < mn) mn = row_vals[a][j+1];
                        if (row_vals[b][j]   < mn) mn = row_vals[b][j];
                        if (row_vals[b][j+1] < mn) mn = row_vals[b][j+1];

                        int mx = row_vals[a][j];
                        if (row_vals[a][j+1] > mx) mx = row_vals[a][j+1];
                        if (row_vals[b][j]   > mx) mx = row_vals[b][j];
                        if (row_vals[b][j+1] > mx) mx = row_vals[b][j+1];

                        int ssum = mn + mx;
                        if (sum_val == -1)
                            sum_val = ssum;
                        else if (sum_val != ssum)
                            ok = false;
                    }
                    if (ok) {
                        adj_mat[sum_val][a][b] = 1;
                    }
                }
            }
        }

        /* Convert adjacency matrices to adjacency lists for faster DP. */
        for (int s = 0; s <= 4; ++s) {
            for (int a = 0; a < N_ROWS; ++a) {
                int cnt = 0;
                for (int b = 0; b < N_ROWS; ++b) {
                    if (adj_mat[s][a][b]) {
                        neigh_list[s][a][cnt++] = (unsigned char)b;
                    }
                }
                neigh_count[s][a] = (unsigned char)cnt;
            }
        }

        initialized = true;
    }

    seq_t total = 0;  /* Result for all sums s = 0…4 */

    /* For each possible sum s perform a DP over rows. */
    for (int s = 0; s <= 4; ++s) {
        /* dp[0] holds current distribution, dp[1] holds next distribution. */
        seq_t dp[2][N_ROWS];
        int cur = 0, nxt = 1;

        /* Start with any single row. */
        for (int i = 0; i < N_ROWS; ++i) {
            dp[cur][i] = (seq_t)1;
        }

        /* Perform n transitions (edges) to build arrays of height n+1. */
        for (idx_t step = 0; step < n; ++step) {
            /* clear next layer */
            for (int i = 0; i < N_ROWS; ++i) {
                dp[nxt][i] = (seq_t)0;
            }

            /* propagate counts using adjacency list */
            for (int a = 0; a < N_ROWS; ++a) {
                seq_t val = dp[cur][a];
                if (val == (seq_t)0) continue;
                int cnt = neigh_count[s][a];
                for (int k = 0; k < cnt; ++k) {
                    int b = neigh_list[s][a][k];
                    dp[nxt][b] += val;
                }
            }

            /* swap layers */
            int tmp = cur;
            cur = nxt;
            nxt = tmp;
        }

        /* Sum all counts after n steps for this s. */
        seq_t sum_s = 0;
        for (int i = 0; i < N_ROWS; ++i) {
            sum_s += dp[cur][i];
        }

        total += sum_s;
    }

    return total;
}
