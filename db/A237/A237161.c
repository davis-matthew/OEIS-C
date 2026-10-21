/*
 * A237161  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   expr_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+1)×3 0..2 arrays with the maximum + the minimum minus the upper median minus the lower median of every 2×2 subblock equal.
   Computed by dynamic programming over the 27 possible rows. For each pair of rows we
   compute the value of that expression; a transition is allowed only if the two
   2×2 blocks in that pair give the same value, and all transitions in an array
   must use the same common value (−2,−1,0,1 or 2).  The total count is the
   sum over all five possible common values of the number of length‑(n+1) row
   sequences using only edges of that value. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/expr_val.h"

/* Precomputed table of the 27 possible rows (each a triple of 0..2). */
static const unsigned char row[27][3] = {
    {0,0,0},{0,0,1},{0,0,2},{0,1,0},{0,1,1},{0,1,2},{0,2,0},{0,2,1},{0,2,2},
    {1,0,0},{1,0,1},{1,0,2},{1,1,0},{1,1,1},{1,1,2},{1,2,0},{1,2,1},{1,2,2},
    {2,0,0},{2,0,1},{2,0,2},{2,1,0},{2,1,1},{2,1,2},{2,2,0},{2,2,1},{2,2,2}
};

/* For each possible common value (−2…2) store which row pairs have that value.
   The index is val+2 to map −2→0 … 2→4. */
static bool edges[5][27][27];
static bool edges_ready = false;

/* Build the transition tables once. */
static void build_edges(void)
{
    if (edges_ready) return;
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 27; ++j) {
            const unsigned char *ri = row[i];
            const unsigned char *rj = row[j];
            int e1 = expr_val(ri[0], ri[1], rj[0], rj[1]);  // block columns 1‑2
            int e2 = expr_val(ri[1], ri[2], rj[1], rj[2]);  // block columns 2‑3
            if (e1 == e2 && e1 >= -2 && e1 <= 2) {
                edges[e1 + 2][i][j] = true;
            }
        }
    }
    edges_ready = true;
}

seq_t a(idx_t n)
{
    build_edges();
    if (n == 0) return (seq_t)0;      /* offset starts at 1; value for n<1 unused */

    /* DP for each common value and sum over them. */
    seq_t total = (seq_t)0;
    /* temporary arrays for DP; 27 is small, so stack allocation is fine. */
    seq_t curr[27];
    seq_t next[27];

    /* For each possible common value (-2..2). */
    for (int val_idx = 0; val_idx < 5; ++val_idx) {
        /* Initialize: one way for each starting row. */
        for (int i = 0; i < 27; ++i) curr[i] = (seq_t)1;

        /* Perform n transitions (to get n+1 rows). */
        for (idx_t step = 0; step < n; ++step) {
            for (int j = 0; j < 27; ++j) {
                seq_t sum = (seq_t)0;
                for (int i = 0; i < 27; ++i) {
                    if (edges[val_idx][i][j]) sum += curr[i];
                }
                next[j] = sum;
            }
            /* Copy next to curr for next iteration. */
            for (int i = 0; i < 27; ++i) curr[i] = next[i];
        }

        /* Add all paths ending at any row. */
        seq_t sum_paths = (seq_t)0;
        for (int i = 0; i < 27; ++i) sum_paths += curr[i];
        total += sum_paths;
    }

    return total;
}
