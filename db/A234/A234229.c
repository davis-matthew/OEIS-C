/*
 * A234229  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Array of (n+1)×3 matrices with the 2×2 subblock diagonal–antidiagonal
   difference equal to 3; the number of such arrays is the number of
   walks of length n in the graph whose vertices are the 512 possible
   rows (triples of base‑8 digits) and whose edges connect rows that
   satisfy the two 2×2 conditions.  The function below constructs the
   graph once and then counts walks by dynamic programming. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* number of possible rows: 8³ = 512 */
enum { ROWS = 512 };

/* adjacency lists (built once) */
static int *adj[ROWS];
static int adj_deg[ROWS];
static bool initialized = false;

/* store base‑8 digits of each row for fast access */
static unsigned char d0[ROWS], d1[ROWS], d2[ROWS];

/* adjacency structure initialisation */
static void init_graph(void)
{
    /* pre‑compute the digits of every row */
    for (int r = 0; r < ROWS; ++r) {
        d0[r] = (r >> 6) & 7;
        d1[r] = (r >> 3) & 7;
        d2[r] =  r        & 7;
    }

    /* first pass: count neighbours for each row */
    for (int r = 0; r < ROWS; ++r) {
        int cnt = 0;
        unsigned char a = d0[r], b = d1[r], c = d2[r];
        for (int rr = 0; rr < ROWS; ++rr) {
            unsigned char ap = d0[rr], bp = d1[rr], cp = d2[rr];
            int diff0 = (int)a + bp - (int)b - ap;
            if (diff0 < 0) diff0 = -diff0;
            if (diff0 != 3) continue;
            int diff1 = (int)b + cp - (int)c - bp;
            if (diff1 < 0) diff1 = -diff1;
            if (diff1 != 3) continue;
            ++cnt;
        }
        adj_deg[r] = cnt;
    }

    /* second pass: store the neighbour lists */
    for (int r = 0; r < ROWS; ++r) {
        if (adj_deg[r] == 0) {
            adj[r] = NULL;
            continue;
        }
        adj[r] = (int*)malloc(adj_deg[r] * sizeof(int));
        int idx = 0;
        unsigned char a = d0[r], b = d1[r], c = d2[r];
        for (int rr = 0; rr < ROWS; ++rr) {
            unsigned char ap = d0[rr], bp = d1[rr], cp = d2[rr];
            int diff0 = (int)a + bp - (int)b - ap;
            if (diff0 < 0) diff0 = -diff0;
            if (diff0 != 3) continue;
            int diff1 = (int)b + cp - (int)c - bp;
            if (diff1 < 0) diff1 = -diff1;
            if (diff1 != 3) continue;
            adj[r][idx++] = rr;
        }
    }

    initialized = true;
}

/* static arrays used for dynamic programming */
static seq_t dp_prev_static[ROWS];
static seq_t dp_curr_static[ROWS];

seq_t a(idx_t n)
{
    /* build graph once */
    if (!initialized) {
        init_graph();
    }

    /* initialise to one for each possible first row */
    for (int r = 0; r < ROWS; ++r) {
        dp_prev_static[r] = (seq_t)1;
    }

    seq_t *dp_prev = dp_prev_static;
    seq_t *dp_curr = dp_curr_static;

    /* perform n transitions (array has n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero current counts */
        for (int r = 0; r < ROWS; ++r) {
            dp_curr[r] = (seq_t)0;
        }

        for (int r = 0; r < ROWS; ++r) {
            seq_t val = dp_prev[r];
            if (val == (seq_t)0) continue;

            int deg = adj_deg[r];
            if (deg == 0) continue;
            int *nbr = adj[r];
            for (int k = 0; k < deg; ++k) {
                int rr = nbr[k];
                dp_curr[rr] = dp_curr[rr] + val;
            }
        }

        /* swap dp_prev and dp_curr for next step */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* sum all walks of length n+1 over all ending rows */
    seq_t total = (seq_t)0;
    for (int r = 0; r < ROWS; ++r) {
        total = total + dp_prev[r];
    }

    return total;
}
