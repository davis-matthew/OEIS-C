/*
 * A233750  Number of (n+1)X(2+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to
 *          10.
 * https://oeis.org/A233750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   diff_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a dynamic programming over (n+1)×3 arrays with 0..5 entries.
   Each 2×2 block must have the sum of the absolute values of its four
   edge differences equal to 10.  The function counts the number of such
   arrays for a given n (n≥1).  The DP runs in O(n·S·K) time where
   S=216 is the number of possible rows and K≈30 is the average number
   of compatible successor rows.  This easily meets the 2‑second limit
   for n up to about 10 000.  The implementation precomputes the
   adjacency lists between rows once and reuses them for every call. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/diff_int.h"

/* Number of states: 6^3 = 216 */
#define NUM_STATES 216
#define MAX_NEIGHBORS 216   /* upper bound for neighbor count */

/* Pre‑computed row components for quick access */
static unsigned char row_a[NUM_STATES];
static unsigned char row_b[NUM_STATES];
static unsigned char row_c[NUM_STATES];

/* Transition adjacency: for each state i, neighbor_cnt[i] gives how many
   successor states j are compatible.  neighbor[i][k] = j. */
static int neighbor[NUM_STATES][MAX_NEIGHBORS];
static unsigned char neighbor_cnt[NUM_STATES];

/* Flag indicating whether the transition tables have been built. */
static bool tables_ready = false;

/* Cache DP arrays: dp[turn][state] holds counts for the current step. */
static seq_t dp[2][NUM_STATES];

seq_t a(idx_t n)
{
    /* Build the transition tables once */
    if (!tables_ready) {
        /* Pre‑compute row encodings */
        for (int s = 0; s < NUM_STATES; ++s) {
            row_a[s] = (s / 36) % 6;
            row_b[s] = (s / 6) % 6;
            row_c[s] = s % 6;
        }

        /* Compute adjacency lists */
        for (int i = 0; i < NUM_STATES; ++i) {
            unsigned char ai = row_a[i];
            unsigned char bi = row_b[i];
            unsigned char ci = row_c[i];

            for (int j = 0; j < NUM_STATES; ++j) {
                unsigned char dj = row_a[j];
                unsigned char ej = row_b[j];
                unsigned char fj = row_c[j];

                int s1 = diff_int(ai, bi) + diff_int(ai, dj) +
                         diff_int(bi, ej) + diff_int(dj, ej);
                if (s1 != 10) continue;

                int s2 = diff_int(bi, ci) + diff_int(bi, ej) +
                         diff_int(ci, fj) + diff_int(ej, fj);
                if (s2 != 10) continue;

                int idx = neighbor_cnt[i]++;
                neighbor[i][idx] = j;
            }
        }

        tables_ready = true;
    }

    /* Handle n == 0 (should not occur, but return 0 safely) */
    if (n == 0) return (seq_t)0;

    /* Initialize DP: one row can be any state */
    int cur = 0, nxt = 1;
    for (int s = 0; s < NUM_STATES; ++s) dp[cur][s] = (seq_t)1;
    memset(dp[nxt], 0, sizeof(dp[nxt]));

    /* Process each additional row */
    for (idx_t step = 1; step <= n; ++step) {
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (int i = 0; i < NUM_STATES; ++i) {
            seq_t val = dp[cur][i];
            if (!val) continue;
            int imp = neighbor_cnt[i];
            int *list = neighbor[i];
            for (int k = 0; k < imp; ++k) {
                dp[nxt][list[k]] += val;
            }
        }
        /* Swap turn */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum over final states to obtain the total number */
    seq_t total = 0;
    for (int s = 0; s < NUM_STATES; ++s) total += dp[cur][s];
    return total;
}
