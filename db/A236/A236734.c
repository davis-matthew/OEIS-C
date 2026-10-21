/*
 * A236734  Number of (n+1)X(5+1) 0..2 arrays with the upper median minus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A236734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 tables with entries 0..2 such that the
 *            difference between the upper and lower median of every 2×2
 *            subblock is the same across the whole table
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Since the function may be called many times, keep a flag so that the
   transfer‑matrix is constructed only once. */
#define NUM_STATES   729          /* 3^6 possible rows */
#define NUM_COLS     6
#define NUM_BLOCKS   5
#define MAX_TRANS    500          /* maximum possible out‑degree (safety margin) */

static uint8_t row_digits[NUM_STATES][NUM_COLS];
static int      adj[3][NUM_STATES][MAX_TRANS];
static uint16_t deg[3][NUM_STATES];
static bool     matrix_ready = false;

/* Build the row representations and the adjacency lists for the different
   medians. */
static void build_transfer_matrix(void)
{
    if (matrix_ready) return;

    /* precompute the base‑3 digits of all rows */
    for (int p = 0; p < NUM_STATES; ++p) {
        int x = p;
        for (int c = 0; c < NUM_COLS; ++c) {
            row_digits[p][c] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* enumerate all ordered pairs of rows */
    for (int p = 0; p < NUM_STATES; ++p) {
        for (int q = 0; q < NUM_STATES; ++q) {
            bool ok[3] = { true, true, true }; /* ok[d] for d=0,1,2 */

            for (int j = 0; j < NUM_BLOCKS && (ok[0] || ok[1] || ok[2]); ++j) {
                int vals[4];
                vals[0] = row_digits[p][j];
                vals[1] = row_digits[p][j + 1];
                vals[2] = row_digits[q][j];
                vals[3] = row_digits[q][j + 1];

                /* simple bubble sort for 4 elements */
                for (int a = 0; a < 4; ++a)
                    for (int b = a + 1; b < 4; ++b)
                        if (vals[a] > vals[b]) {
                            int t = vals[a];
                            vals[a] = vals[b];
                            vals[b] = t;
                        }

                int diff = vals[2] - vals[1];  /* U - L */
                if (diff != 0) ok[0] = false;
                if (diff != 1) ok[1] = false;
                if (diff != 2) ok[2] = false;
            }

            for (int d = 0; d < 3; ++d) {
                if (ok[d]) {
                    adj[d][p][deg[d][p]++] = q;
                }
            }
        }
    }

    matrix_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* the sequence is defined for n ≥ 1 */

    build_transfer_matrix();

    seq_t total = (seq_t)0;

    /* arrays for dynamic programming; reused for each d because they are small */
    seq_t dp[NUM_STATES];
    seq_t dp_next[NUM_STATES];

    for (int d = 0; d < 3; ++d) {

        /* initialise dp : any first row is allowed */
        for (int i = 0; i < NUM_STATES; ++i) dp[i] = (seq_t)1;

        /* perform n transitions (there are n 2×2 blocks vertically) */
        for (idx_t step = 0; step < n; ++step) {

            /* clear next layer */
            for (int i = 0; i < NUM_STATES; ++i) dp_next[i] = (seq_t)0;

            /* propagate counts along the edges of the transfer matrix */
            for (int p = 0; p < NUM_STATES; ++p) {
                seq_t val = dp[p];
                if (val == (seq_t)0) continue;

                int out_deg = deg[d][p];
                for (int e = 0; e < out_deg; ++e) {
                    int q = adj[d][p][e];
                    dp_next[q] += val;
                }
            }

            /* swap dp and dp_next for the next step */
            for (int i = 0; i < NUM_STATES; ++i) dp[i] = dp_next[i];
        }

        /* sum over all final states to obtain the number of arrays with this d */
        seq_t sum_d = (seq_t)0;
        for (int i = 0; i < NUM_STATES; ++i) sum_d += dp[i];

        total += sum_d;
    }

    return total;
}
