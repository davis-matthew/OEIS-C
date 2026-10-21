/*
 * A236886  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          upper median minus the lower median minus the minimum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A236886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays with entries 0..2 such that *
 *            for every 2×2 subblock the value * max + upper_median -
 *            lower_median - min * is the same across all subblocks. *
 *            Computation is done by dynamic programming over the 9 possible
 *            row * patterns, treating the transition graph for each poss...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code is ISO C11 / C++17 compliant. */
#include <stdbool.h>
#include <stdlib.h>

static const int row_vals[9][2] = {
    {0,0},{0,1},{0,2},
    {1,0},{1,1},{1,2},
    {2,0},{2,1},{2,2}
};

/* Helper that computes the block value for two adjacent rows. */
static inline int block_value(int a0, int a1, int b0, int b1)
{
    int v[4] = {a0, a1, b0, b1};
    /* simple insertion sort for 4 elements */
    for (int i = 1; i < 4; ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
    return v[3] + v[2] - v[1] - v[0];
}

/* Build the six adjacency tables once. */
static void build_adjacency(bool adj[5][9][9])
{
    for (int e = 0; e < 5; ++e) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                int val = block_value(row_vals[i][0], row_vals[i][1],
                                      row_vals[j][0], row_vals[j][1]);
                adj[e][i][j] = (val == e);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset is 1 */

    /* Static adjacency tables */
    static bool adjacency[5][9][9];
    static bool adj_init = false;
    if (!adj_init) {
        build_adjacency(adjacency);
        adj_init = true;
    }

    /* DP arrays: prev and next for each block value (0..4) and each row (0..8) */
    seq_t prev[5][9];
    seq_t next[5][9];

    /* Initialize for step 0 (zero edges: just a single row). */
    for (int e = 0; e < 5; ++e)
        for (int r = 0; r < 9; ++r)
            prev[e][r] = (seq_t)1;

    /* Perform n steps (each step corresponds to adding one more row). */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next */
        for (int e = 0; e < 5; ++e)
            for (int r = 0; r < 9; ++r)
                next[e][r] = (seq_t)0;

        for (int e = 0; e < 5; ++e) {
            for (int curr = 0; curr < 9; ++curr) {
                seq_t cur_val = prev[e][curr];
                if (cur_val == 0)
                    continue;
                for (int nxt = 0; nxt < 9; ++nxt) {
                    if (adjacency[e][curr][nxt])
                        next[e][nxt] += cur_val;
                }
            }
        }
        /* swap prev and next for next iteration */
        for (int e = 0; e < 5; ++e)
            for (int r = 0; r < 9; ++r)
                prev[e][r] = next[e][r];
    }

    /* Sum over all final rows and all block values to get the count. */
    seq_t result = (seq_t)0;
    for (int e = 0; e < 5; ++e)
        for (int r = 0; r < 9; ++r)
            result += prev[e][r];

    return result;
}
