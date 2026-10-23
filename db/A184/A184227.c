/*
 * A184227  Half the number of (n+1) X 6 0..3 arrays with every 2 X 2 subblock
 *          diagonal sum differing from its antidiagonal sum by more than 3.
 * https://oeis.org/A184227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) · Σ_{i,j} (A^n)[i][j] * where A is the 4096×4096
 *            adjacency matrix for rows of length 6 over {0,1,2,3} * with the
 *            property that for each 2×2 subblock the absolute difference * of
 *            the two possible diagonal sums is > 3. * The sequence is
 *            computed by dynamic programming on the directed g...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROWS 4096  /* 4^6 */

static int      *adj_start = NULL;
static int      *adj_len   = NULL;
static int      *adj_buf   = NULL;
static bool     built = false;

/* Precompute the list of admissible successors for each row. */
static void build_adj(void)
{
    int outcnt[NUM_ROWS];
    int total_edges = 0;
    int digits[NUM_ROWS][6];

    /* pre‑compute digits of every row */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int x = r;
        for (int k = 0; k < 6; ++k) {
            digits[r][k] = x & 3;   /* x % 4 */
            x >>= 2;               /* x /= 4 */
        }
    }

    /* first pass: count outgoing edges */
    for (int a = 0; a < NUM_ROWS; ++a) {
        outcnt[a] = 0;
        for (int b = 0; b < NUM_ROWS; ++b) {
            /* test the 5 overlapping 2×2 blocks */
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                int d1 = digits[a][j] + digits[b][j+1];
                int d2 = digits[a][j+1] + digits[b][j];
                int diff = d1 - d2;
                if (diff < 0) diff = -diff;
                if (diff <= 3) ok = false;
            }
            if (ok) {
                ++outcnt[a];
                ++total_edges;
            }
        }
    }

    /* allocate structures */
    adj_start = (int*)malloc(NUM_ROWS * sizeof(int));
    adj_len   = (int*)malloc(NUM_ROWS * sizeof(int));
    adj_buf   = (int*)malloc(total_edges * sizeof(int));

    /* compute start positions via prefix sums */
    adj_start[0] = 0;
    for (int i = 0; i < NUM_ROWS; ++i) {
        adj_len[i] = outcnt[i];
        if (i + 1 < NUM_ROWS)
            adj_start[i+1] = adj_start[i] + outcnt[i];
    }

    /* second pass: fill adjacency list */
    int cur_pos[NUM_ROWS];
    memcpy(cur_pos, adj_start, NUM_ROWS * sizeof(int));
    for (int a = 0; a < NUM_ROWS; ++a) {
        for (int b = 0; b < NUM_ROWS; ++b) {
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                int d1 = digits[a][j] + digits[b][j+1];
                int d2 = digits[a][j+1] + digits[b][j];
                int diff = d1 - d2;
                if (diff < 0) diff = -diff;
                if (diff <= 3) ok = false;
            }
            if (ok) {
                int idx = cur_pos[a]++;
                adj_buf[idx] = b;
            }
        }
    }

    /* mark built */
    built = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* ensure adjacency is initialized */
    if (!built) build_adj();

    /* dynamic programming arrays */
    seq_t dp[NUM_ROWS];
    seq_t ndp[NUM_ROWS];

    /* initialize dp to 1 (any first row) */
    for (int i = 0; i < NUM_ROWS; ++i) {
        dp[i] = (seq_t)1;
        ndp[i] = (seq_t)0;
    }

    /* iterate n steps; each step adds a row */
    for (idx_t step = 0; step < n; ++step) {
        /* propagate counts to successors */
        for (int i = 0; i < NUM_ROWS; ++i) {
            seq_t val = dp[i];
            if (!val) continue;
            int start = adj_start[i];
            int end   = start + adj_len[i];
            for (int idx = start; idx < end; ++idx) {
                int v = adj_buf[idx];
                ndp[v] += val;
            }
        }
        /* swap dp and ndp, reset ndp */
        for (int i = 0; i < NUM_ROWS; ++i) {
            dp[i] = ndp[i];
            ndp[i] = (seq_t)0;
        }
    }

    /* sum all counts to get total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_ROWS; ++i) {
        total += dp[i];
    }

    /* divide by 2 to obtain the requested sequence value */
    return total >> 1;  /* works for all supported seq_t implementations */
}
