/*
 * A205904  Number of (n+1)X4 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock the same.
 * https://oeis.org/A205904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A205904 (Number of (n+1)×4 arrays with uniform clockwise edge
 * increases in all 2×2 subblocks).  The algorithm builds the 81 possible rows
 * (each 4 cells taking values 0,1,2) and finds, for every k=0..4, the pairs of
 * rows that produce the same number k of clockwise increases in the three
 * adjacent 2×2 subblocks of that row pair.  It then performs dynamic
 * programming over the row sequence: for a given k, the number of length‑n+1
 * arrays equals the number of walks of length n in the graph of admissible
 * row transitions.  The total answer is the sum over all k.  All arithmetic
 * is done in the provided type seq_t.  The implementation uses adjacency
 * lists and memoises the graph construction. */
#include <stdbool.h>
#include <stddef.h>

/* ---------- Helper for 4‑digit base‑3 row representation ---------- */
static uint8_t row_digits[81][4];
static void init_row_digits(void)
{
    for (int r = 0; r < 81; ++r) {
        int x = r;
        for (int d = 0; d < 4; ++d) {
            row_digits[r][d] = x % 3;
            x /= 3;
        }
    }
}

/* ---------- Adjacency information ---------- */
#define MAX_ROWS 81
#define MAX_K 5
static uint8_t deg[MAX_K][MAX_ROWS];                 /* number of neighbours */
static uint8_t edges[MAX_K][MAX_ROWS][MAX_ROWS];    /* neighbour list */
static bool graph_built = false;

static void build_graph(void)
{
    init_row_digits();

    /* clear deg arrays */
    for (int k = 0; k < MAX_K; ++k)
        for (int i = 0; i < MAX_ROWS; ++i)
            deg[k][i] = 0;

    /* examine all ordered row pairs */
    for (int u = 0; u < MAX_ROWS; ++u) {
        const uint8_t *du = row_digits[u];
        for (int v = 0; v < MAX_ROWS; ++v) {
            const uint8_t *dv = row_digits[v];
            int inc[3];
            for (int j = 0; j < 3; ++j) {
                uint8_t a = du[j];
                uint8_t b = du[j + 1];
                uint8_t c = dv[j];
                uint8_t d = dv[j + 1];
                int cnt = 0;
                if (a < b) ++cnt;          /* right */
                if (b < d) ++cnt;          /* down */
                if (d < c) ++cnt;          /* left */
                if (c < a) ++cnt;          /* up */
                inc[j] = cnt;
            }
            if (inc[0] == inc[1] && inc[1] == inc[2]) {
                int k = inc[0];
                uint8_t dcnt = deg[k][u];
                edges[k][u][dcnt] = (uint8_t)v;
                ++deg[k][u];
            }
        }
    }
    graph_built = true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!graph_built)
        build_graph();

    if (n == 0)            /* offset starts at 1; behaviour for n=0 is arbitrary */
        return 0;

    const idx_t steps = n; /* number of transitions = n (array has n+1 rows) */

    seq_t total = 0;
    seq_t dp[MAX_ROWS], nxt[MAX_ROWS];

    for (int k = 0; k < 5; ++k) {
        /* initialise dp to 1 for all starting rows */
        for (int i = 0; i < MAX_ROWS; ++i)
            dp[i] = 1;

        for (idx_t step = 0; step < steps; ++step) {
            /* clear next */
            for (int i = 0; i < MAX_ROWS; ++i)
                nxt[i] = 0;

            /* transition */
            for (int i = 0; i < MAX_ROWS; ++i) {
                seq_t val = dp[i];
                if (val == 0)
                    continue;
                uint8_t dcnt = deg[k][i];
                for (uint8_t t = 0; t < dcnt; ++t) {
                    uint8_t v = edges[k][i][t];
                    nxt[v] += val;
                }
            }

            /* copy back */
            for (int i = 0; i < MAX_ROWS; ++i)
                dp[i] = nxt[i];
        }

        /* sum over all ending rows */
        seq_t sum_k = 0;
        for (int i = 0; i < MAX_ROWS; ++i)
            sum_k += dp[i];

        total += sum_k;
    }

    return total;
}
