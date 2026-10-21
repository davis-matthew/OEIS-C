/*
 * A224743  Number of (n+1)X7 0..1 matrices with each 2X2 permanent equal.
 * https://oeis.org/A224743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224743
 *  Number of (n+1)×7 binary matrices whose every 2×2 permanent is
 *  equal.  The value a(n) is obtained by dynamic programming on a
 *  graph over the 7‑bit rows.  For each permanent value c∈{0,1,2}
 *  an undirected edge (v,w) is created iff all six 2×2 permanents
 *  formed by the rows v,w and the consecutive columns are equal to
 *  c.  Any matrix corresponds to a walk of length n in one of the
 *  three graphs, and the total number of walks (starting at any
 *  vertex) gives a(n).  The routine memoises the results up to the
 *  largest n requested, so repeated calls are fast.  No external
 *  libraries are required beyond the standard headers.  All
 *  calculations are performed in seq_t; intermediate values never
 *  exceed the maximum representable seq_t value for the given build.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* constants ------------------------------------------------------------- */
#define N_STATES 128            /* 7‑bit rows: 0 … 127 */
#define N_C 3                   /* permanents 0,1,2 */
#define MAX_PRECOMPUTE 15000    /* enough for typical n‑range */

/* global data ----------------------------------------------------------- */
static bool data_prepared = false;

/* adjacency lists: deg[c][v] = number of neighbours of v in graph c.
   neigh[c][v] is an array of neighbours of v. */
static int deg[N_C][N_STATES];
static int neigh[N_C][N_STATES][N_STATES];

/* DP containers --------------------------------------------------------- */
static seq_t cur[N_C][N_STATES];
static seq_t nxt[N_C][N_STATES];

/* memoised table -------------------------------------------------------- */
static seq_t a_table[MAX_PRECOMPUTE + 1];
static idx_t computed_n = 0;      /* largest n with valid a_table[n] */

/* --------------------------------------------------------------------- */
 /* Build the three graphs the first time the function is called. */
static void prepare_data(void)
{
    if (data_prepared) return;
    data_prepared = true;

    /* initialize degrees and neighbour lists */
    for (int c = 0; c < N_C; ++c)
        for (int v = 0; v < N_STATES; ++v) {
            deg[c][v] = 0;
            /* neigh already zeroed statically */
        }

    /* examine each unordered pair of rows, avoid double counting. */
    for (int a = 0; a < N_STATES; ++a) {
        for (int b = a; b < N_STATES; ++b) {
            int c = -1;
            bool ok = true;
            /* compute permanents for the six overlapping 2×2 windows */
            for (int j = 0; j < 6; ++j) {
                int t = ((a >> j) & 1) * ((b >> (j + 1)) & 1) +
                        ((a >> (j + 1)) & 1) * ((b >> j) & 1);
                if (j == 0) c = t;
                else if (t != c) { ok = false; break; }
            }
            if (!ok || c < 0 || c > 2) continue;

            /* add undirected edge a<->b for permanent c */
            neigh[c][a][deg[c][a]++] = b;
            if (a != b) {
                neigh[c][b][deg[c][b]++] = a;
            } else {  /* self-loop counted once */
                /* already added once above */
            }
        }
    }

    /* initialise DP state: one walk starts from each row in each graph */
    for (int c = 0; c < N_C; ++c)
        for (int v = 0; v < N_STATES; ++v)
            cur[c][v] = 1;

    a_table[0] = 0;   /* meaningless but harmless */
    computed_n = 0;
}

/* --------------------------------------------------------------------- */
 /* Extend the memoised table up to n (inclusive).                       */
static void extend_table(idx_t n)
{
    if (computed_n >= n) return;
    prepare_data();

    for (idx_t step = computed_n + 1; step <= n; ++step) {
        /* compute nxt from cur */
        for (int c = 0; c < N_C; ++c)
            memset(nxt[c], 0, N_STATES * sizeof(seq_t));

        for (int c = 0; c < N_C; ++c) {
            for (int v = 0; v < N_STATES; ++v) {
                seq_t val = cur[c][v];
                if (!val) continue;
                int d = deg[c][v];
                for (int i = 0; i < d; ++i) {
                    int w = neigh[c][v][i];
                    nxt[c][w] += val;
                }
            }
        }

        /* total number of (step+1)×7 matrices for this step */
        seq_t total = 0;
        for (int c = 0; c < N_C; ++c)
            for (int v = 0; v < N_STATES; ++v)
                total += nxt[c][v];

        a_table[step] = total;

        /* move to next iteration */
        for (int c = 0; c < N_C; ++c)
            memcpy(cur[c], nxt[c], N_STATES * sizeof(seq_t));

        computed_n = step;
    }
}

/* --------------------------------------------------------------------- */
 /* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined, return 0 */

    if (n > MAX_PRECOMPUTE) {
        /* For very large n, compute on the fly without memoisation.   */
        prepare_data();

        /* reset DP to initial state */
        for (int c = 0; c < N_C; ++c)
            for (int v = 0; v < N_STATES; ++v)
                cur[c][v] = 1;

        seq_t answer = 0;
        for (idx_t step = 1; step <= n; ++step) {
            for (int c = 0; c < N_C; ++c)
                memset(nxt[c], 0, N_STATES * sizeof(seq_t));

            for (int c = 0; c < N_C; ++c) {
                for (int v = 0; v < N_STATES; ++v) {
                    seq_t val = cur[c][v];
                    if (!val) continue;
                    int d = deg[c][v];
                    for (int i = 0; i < d; ++i) {
                        int w = neigh[c][v][i];
                        nxt[c][w] += val;
                    }
                }
            }

            seq_t step_total = 0;
            for (int c = 0; c < N_C; ++c)
                for (int v = 0; v < N_STATES; ++v)
                    step_total += nxt[c][v];

            answer = step_total;

            for (int c = 0; c < N_C; ++c)
                memcpy(cur[c], nxt[c], N_STATES * sizeof(seq_t));
        }
        return answer;
    }

    extend_table(n);
    return a_table[n];
}
