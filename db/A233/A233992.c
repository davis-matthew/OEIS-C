/*
 * A233992  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 18.
 * https://oeis.org/A233992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   abs_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1)×4 arrays with entries 0..6 such that
 * every 2×2 subblock satisfies  
 * |a-b| + |c-d| + |a-c| + |b-d| + |a-d| + |b-c| = 18.
 * This is computed by dynamic programming on the sparse transition graph
 * of 4‑tuples of values. 
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/abs_int.h"

#define STATES 2401          /* 7^4 */
#define EDGE_CAP (STATES * 7)/* upper bound on number of edges */

/* static storage for transition graph */
static uint8_t row_vals[STATES][4];
static int    succ_start[STATES];
static int    succ_len[STATES];
static int    succ_edges[EDGE_CAP];
static int    edge_cnt = 0;
static int    adj_built = 0;

/* build the transition graph lazily */
static void build_adj(void)
{
    if (adj_built) return;
    adj_built = 1;

    /* encode all 4‑tuples */
    for (int i = 0; i < STATES; ++i)
    {
        int v = i;
        for (int c = 0; c < 4; ++c)
        {
            row_vals[i][c] = v % 7;
            v /= 7;
        }
    }

    /* for each pair (i,j) check the 2×2 condition */
    for (int i = 0; i < STATES; ++i)
    {
        succ_start[i] = edge_cnt;
        int len = 0;
        for (int j = 0; j < STATES; ++j)
        {
            int ok = 1;
            for (int col = 0; col < 3; ++col)
            {
                int r1a = row_vals[i][col];
                int r1b = row_vals[i][col + 1];
                int r2a = row_vals[j][col];
                int r2b = row_vals[j][col + 1];

                int sum = abs_int(r1a - r1b) + abs_int(r2a - r2b)
                        + abs_int(r1a - r2a) + abs_int(r1b - r2b)
                        + abs_int(r1a - r2b) + abs_int(r1b - r2a);
                if (sum != 18)
                {
                    ok = 0;
                    break;
                }
            }
            if (ok)
            {
                succ_edges[edge_cnt++] = j;
                ++len;
            }
        }
        succ_len[i] = len;
    }
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* offsets start at 1 */
    if (n == 0)
        return (seq_t)0;

    build_adj();

    /* two state vectors */
    static seq_t dp[STATES], dp_next[STATES];
    /* initialise dp to 1 (any first row) */
    for (int i = 0; i < STATES; ++i)
        dp[i] = (seq_t)1;

    /* dp over n transitions */
    for (idx_t step = 0; step < n; ++step)
    {
        /* clear dp_next */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate edges */
        for (int i = 0; i < STATES; ++i)
        {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            int start = succ_start[i];
            int len = succ_len[i];
            for (int k = 0; k < len; ++k)
                dp_next[succ_edges[start + k]] += val;
        }
        /* swap dp and dp_next */
        for (int i = 0; i < STATES; ++i)
            dp[i] = dp_next[i];
    }

    /* sum all possible final rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        total += dp[i];

    return total;
}
