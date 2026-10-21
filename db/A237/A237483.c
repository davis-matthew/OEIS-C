/*
 * A237483  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   block_E
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237483:  number of (n+1)×3 arrays with entries 0..3 such that
 * the value (maximum + minimum – lower median) of every 2×2 subblock
 * is the same constant.  Computed by dynamic programming on a
 * 64‑state directed graph built once for all possible 3‑cell rows. */

#include <stdint.h>
#include "../utils/block_E.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Graph definition                                                     */
#define MAX_ROWS     64          /* 3‑cell rows with values 0..3 */
#define MIN_E        -3          /* possible E values: -3 ..  6 */
#define MAX_E        6
#define E_COUNT      (MAX_E - MIN_E + 1)   /* =10 */

static uint8_t rows[MAX_ROWS][3];
static int edges[E_COUNT][MAX_ROWS][MAX_ROWS];
static int edges_len[E_COUNT][MAX_ROWS];
static int graph_initialized = 0;

/* ------------------------------------------------------------------ */
/*  Build directed graph once.  edges[e][i] contains all rows j such
 *  that a transition from row i to row j keeps the same E value e.    */
static void build_graph(void)
{
    if (graph_initialized) return;

    /* enumerate all 3-cell rows */
    for (int i = 0; i < MAX_ROWS; ++i) {
        rows[i][0] = (i >> 4) & 3;
        rows[i][1] = (i >> 2) & 3;
        rows[i][2] = i & 3;
    }

    /* reset adjacency lists */
    for (int e = 0; e < E_COUNT; ++e)
        for (int i = 0; i < MAX_ROWS; ++i)
            edges_len[e][i] = 0;

    /* fill adjacency lists */
    for (int i = 0; i < MAX_ROWS; ++i) {
        uint8_t a = rows[i][0], b = rows[i][1], c = rows[i][2];
        for (int j = 0; j < MAX_ROWS; ++j) {
            uint8_t d = rows[j][0], e2 = rows[j][1], f = rows[j][2];
            int e0 = block_E(a, b, d, e2);
            int e1 = block_E(b, c, e2, f);
            if (e0 == e1) {
                int idx = e0 - MIN_E;            /* shift to 0..9 */
                int k = edges_len[idx][i]++;
                edges[idx][i][k] = j;
            }
        }
    }
    graph_initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Main routine: returns a(n) for n >= 1 (offset 1).                  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* not defined but safe */

    build_graph();

    seq_t total = (seq_t)0;

    static seq_t dp[MAX_ROWS];
    static seq_t newdp[MAX_ROWS];

    for (int e = 0; e < E_COUNT; ++e) {
        /* start with all possible first rows */
        for (int i = 0; i < MAX_ROWS; ++i)
            dp[i] = (seq_t)1;

        /* propagate n steps: each step adds a new row */
        for (idx_t step = 0; step < n; ++step) {
            for (int i = 0; i < MAX_ROWS; ++i)
                newdp[i] = (seq_t)0;

            for (int i = 0; i < MAX_ROWS; ++i) {
                seq_t val = dp[i];
                if (val == (seq_t)0) continue;
                int len = edges_len[e][i];
                for (int k = 0; k < len; ++k) {
                    int j = edges[e][i][k];
                    newdp[j] += val;
                }
            }
            for (int i = 0; i < MAX_ROWS; ++i)
                dp[i] = newdp[i];
        }

        /* sum all paths ending in any row */
        seq_t sum_e = (seq_t)0;
        for (int i = 0; i < MAX_ROWS; ++i)
            sum_e += dp[i];
        total += sum_e;
    }

    return total;
}
