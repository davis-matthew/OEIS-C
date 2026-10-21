/*
 * A236878  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} whose * every 2×2
 *            subblock has equal sums of * (maximum + upper median + lower
 *            median) * for the left and right subblocks. * * The
 *            implementation builds a directed graph on the 27 possible * rows
 *            (3‑tuples over {0,1,2}) and an adjacency matrix for each...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   max_plus_medians
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/max_plus_medians.h"

#define R 27          /* number of possible rows (3‑tuples) */
#define S_MAX 6       /* S ranges from 0 to 6 */

static unsigned char rows[R][3];          /* pre‑computed rows */
static bool adj[S_MAX + 1][R][R];          /* adjacency for each S */
static bool initialized = false;

/* Build the rows array: rows[i] is the base‑3 representation of i. */
static void build_rows(void)
{
    for (int r = 0; r < R; ++r) {
        int v = r;
        for (int k = 2; k >= 0; --k) {
            rows[r][k] = (unsigned char)(v % 3);
            v /= 3;
        }
    }
}

/* Build adjacency matrices for all possible S. */
static void build_adj(void)
{
    for (int i = 0; i < R; ++i) {
        int a0 = rows[i][0];
        int a1 = rows[i][1];
        int a2 = rows[i][2];
        for (int j = 0; j < R; ++j) {
            int b0 = rows[j][0];
            int b1 = rows[j][1];
            int b2 = rows[j][2];

            int left  = max_plus_medians(a0, a1, b0, b1);
            int right = max_plus_medians(a1, a2, b1, b2);

            if (left == right) {
                adj[left][i][j] = true;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_rows();
        build_adj();
        initialized = true;
    }
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    seq_t ans = (seq_t)0;
    seq_t dp_prev[R];
    seq_t dp_curr[R];

    for (int S = 0; S <= S_MAX; ++S) {
        /* start in any row */
        for (int i = 0; i < R; ++i) dp_prev[i] = (seq_t)1;

        /* perform n transitions (n steps) */
        for (idx_t step = 1; step <= n; ++step) {
            /* clear dp_curr */
            for (int k = 0; k < R; ++k) dp_curr[k] = (seq_t)0;

            for (int i = 0; i < R; ++i) {
                seq_t val = dp_prev[i];
                if (val == (seq_t)0) continue;
                for (int j = 0; j < R; ++j) {
                    if (adj[S][i][j]) {
                        dp_curr[j] += val;
                    }
                }
            }
            /* move to next step */
            for (int k = 0; k < R; ++k) dp_prev[k] = dp_curr[k];
        }

        /* add total number of walks for this S */
        for (int i = 0; i < R; ++i) ans += dp_prev[i];
    }
    return ans;
}
