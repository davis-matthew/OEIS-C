/*
 * A205912  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          the same number of clockwise edge increases as its horizontal
 *          neighbors and the same number of counterclockwise edge increases
 *          as its vertical neighbors.
 * https://oeis.org/A205912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205912.  The implementation builds the directed graph of
   admissible row transitions.  Each vertex is a 4‑digit base‑3 row
   (81 rows).  An edge (f → t) is present iff the 2×2 blocks formed by
   rows f and t have the same number of clockwise edge increases in all
   3 positions (columns j, j+1) and produce the same counter‑clockwise
   vector (cc0,cc1,cc2).  For a fixed counter‑clockwise vector C the graph
   remains unchanged; rows are allowed to form a walk of length n (the
   array has n+1 rows).  The number of such walks is the desired
   sequence term.  No closed‑form formula is known.  */



#include <stdint.h>
#include <stdbool.h>

static uint8_t row_digits[81][4];
static uint8_t edges_by_cc[125][81][81];
static uint8_t edges_len[125][81];
static seq_t dp[125][81];
static seq_t ndp[125][81];

/* One‑time initialization of the graph, called lazily on first use. */
static void init_graph(void)
{
    static bool started = false;
    if (started) return;
    started = true;

    /* Precompute the digits of each row 0…80. */
    for (int r = 0; r < 81; ++r) {
        int tmp = r;
        for (int j = 0; j < 4; ++j) {
            row_digits[r][j] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
    }

    /* Initialize edge counts. */
    for (int cc = 0; cc < 125; ++cc)
        for (int f = 0; f < 81; ++f)
            edges_len[cc][f] = 0;

    /* Build the graph: for each pair of rows. */
    for (int f = 0; f < 81; ++f) {
        for (int t = 0; t < 81; ++t) {
            int cw[3];
            int cc_cnt[3];
            for (int j = 0; j < 3; ++j) {
                uint8_t a = row_digits[f][j];
                uint8_t b = row_digits[f][j+1];
                uint8_t c = row_digits[t][j];
                uint8_t d = row_digits[t][j+1];

                /* Clockwise edges. */
                int inc1 = (b > a);
                int inc2 = (d > b);
                int inc3 = (c > d);
                int inc4 = (a > c);
                cw[j] = inc1 + inc2 + inc3 + inc4;

                /* Counter‑clockwise edges. */
                int ic1 = (c > a);
                int ic2 = (d > c);
                int ic3 = (b > d);
                int ic4 = (a > b);
                cc_cnt[j] = ic1 + ic2 + ic3 + ic4;
            }
            /* Ensure clockwise counts are constant across columns. */
            if (cw[0] == cw[1] && cw[1] == cw[2]) {
                int cc_idx = cc_cnt[0]*25 + cc_cnt[1]*5 + cc_cnt[2];
                edges_by_cc[cc_idx][f][edges_len[cc_idx][f]++] = (uint8_t)t;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_graph();

    /* Reset DP tables. */
    for (int cc = 0; cc < 125; ++cc)
        for (int r = 0; r < 81; ++r)
            dp[cc][r] = 0;

    /* Length 1 (n==1): count all transitions. */
    for (int f = 0; f < 81; ++f) {
        for (int cc = 0; cc < 125; ++cc) {
            for (int k = 0; k < edges_len[cc][f]; ++k) {
                uint8_t to = edges_by_cc[cc][f][k];
                dp[cc][to] += (seq_t)1;
            }
        }
    }

    /* For n > 1: perform (n-1) further extensions. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Clear ndp. */
        for (int cc = 0; cc < 125; ++cc)
            for (int r = 0; r < 81; ++r)
                ndp[cc][r] = 0;

        /* Transition step. */
        for (int cc = 0; cc < 125; ++cc) {
            for (int f = 0; f < 81; ++f) {
                seq_t val = dp[cc][f];
                if (val == 0) continue;
                for (int k = 0; k < edges_len[cc][f]; ++k) {
                    uint8_t to = edges_by_cc[cc][f][k];
                    ndp[cc][to] += val;
                }
            }
        }

        /* Swap dp and ndp. */
        for (int cc = 0; cc < 125; ++cc)
            for (int r = 0; r < 81; ++r)
                dp[cc][r] = ndp[cc][r];
    }

    /* Sum all counts for the final answer. */
    seq_t result = 0;
    for (int cc = 0; cc < 125; ++cc)
        for (int r = 0; r < 81; ++r)
            result += dp[cc][r];

    return result;
}
