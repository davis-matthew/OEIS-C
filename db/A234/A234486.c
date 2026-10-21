/*
 * A234486  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×5 arrays with entries 0..3 such that
 *            every 2×2 subblock has its diagonal sum differing from its
 *            antidiagonal sum by exactly 2. The value is computed by dynamic
 *            programming on the state graph formed by all possible rows (4^5
 *            = 1024)
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define STATES 1024   /* 4^5 possible row configurations */

static uint8_t row_vals[STATES][5];          /* row_vals[s][c]: cell value of row s at column c (0..4) */
static uint16_t neigh_cnt[STATES];           /* number of allowed next states */
static uint16_t neighbor[STATES][STATES];    /* adjacency list for each state */
static bool graph_built = false;

/* Builds the state graph once. */
static void build_graph(void)
{
    /* Pre‑compute the cell values of every row. */
    for (int s = 0; s < STATES; ++s) {
        int tmp = s;
        for (int c = 4; c >= 0; --c) {
            row_vals[s][c] = (uint8_t)(tmp & 3);
            tmp >>= 2;
        }
    }

    /* Build adjacency lists. */
    for (int from = 0; from < STATES; ++from) {
        uint16_t cnt = 0;
        for (int to = 0; to < STATES; ++to) {
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int a = row_vals[from][col];
                int b = row_vals[from][col + 1];
                int c = row_vals[to][col];
                int d = row_vals[to][col + 1];
                int diff = (a + d) - (b + c);
                if (diff != 2 && diff != -2) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                neighbor[from][cnt++] = (uint16_t)to;
            }
        }
        neigh_cnt[from] = cnt;
    }

    graph_built = true;
}

/* Computes a(n). */
seq_t a(idx_t n)
{
    if (!graph_built) build_graph();

    /* DP vectors: number of paths ending in each state after the current number of rows. */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];

    /* Base: one way to form an array consisting of a single row (height 1). */
    for (int s = 0; s < STATES; ++s) {
        cur[s] = (seq_t)1;
        nxt[s] = (seq_t)0;
    }

    /* Perform n transitions → arrays of height n+1. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear next vector. */
        for (int s = 0; s < STATES; ++s) {
            nxt[s] = (seq_t)0;
        }

        /* Propagate counts through allowed transitions. */
        for (int from = 0; from < STATES; ++from) {
            seq_t ways = cur[from];
            if (ways == (seq_t)0) continue;
            uint16_t cnt = neigh_cnt[from];
            uint16_t *list = neighbor[from];
            for (uint16_t k = 0; k < cnt; ++k) {
                uint16_t to = list[k];
                nxt[to] += ways;
            }
        }

        /* Prepare for next iteration. */
        for (int s = 0; s < STATES; ++s) {
            cur[s] = nxt[s];
        }
    }

    /* Sum over all final states to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATES; ++s) {
        total += cur[s];
    }
    return total;
}
