/*
 * A204223  Number of (n+1)X2 0..2 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over rows. Each (n+1)×2 array of
 *            0..2 entries corresponds to a sequence of rows r0,r1,…,rn. Let
 *            S(i)=sum of entries in rows i and i+1. The array satisfies the
 *            problem condition iff S(0)≤S(1)≤…≤S(n-1). The DP enumerates all
 *            such sequences efficiently
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The required function. */
seq_t a(idx_t n)
{
    /* Pre‑compute the sum of the two entries of each possible row
       (there are 3^2 = 9 rows).  A row is encoded as 3*x + y where
       x,y∈{0,1,2}. */
    static const int row_sum[9] = {
        0, /* 0,0 */
        1, /* 0,1 */
        2, /* 0,2 */
        1, /* 1,0 */
        2, /* 1,1 */
        3, /* 1,2 */
        2, /* 2,0 */
        3, /* 2,1 */
        4  /* 2,2 */
    };

    /* DP arrays:
       dp_cur[row][sum]   = number of sequences of current length
       ending with ‘row’ and last 2×2 block sum = ‘sum’.
       sum ranges 0..8 (inclusive). */
    seq_t dp_cur[9][9];
    seq_t dp_next[9][9];

    /* Initialise dp_cur for sequences of length 2 (two rows). */
    for (int r = 0; r < 9; ++r)
        for (int s = 0; s < 9; ++s)
            dp_cur[r][s] = 0;

    for (int r0 = 0; r0 < 9; ++r0) {
        for (int r1 = 0; r1 < 9; ++r1) {
            int sum = row_sum[r0] + row_sum[r1];   /* 0..8 */
            dp_cur[r1][sum] += 1;
        }
    }

    /* Step through the remaining rows (from the third row up to row n). */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dp_next. */
        for (int r = 0; r < 9; ++r)
            for (int s = 0; s < 9; ++s)
                dp_next[r][s] = 0;

        /* Transition: append a new row. */
        for (int prev_row = 0; prev_row < 9; ++prev_row) {
            for (int prev_sum = 0; prev_sum < 9; ++prev_sum) {
                seq_t cnt = dp_cur[prev_row][prev_sum];
                if (!cnt) continue;
                for (int new_row = 0; new_row < 9; ++new_row) {
                    int new_sum = row_sum[prev_row] + row_sum[new_row];
                    if (new_sum >= prev_sum)
                        dp_next[new_row][new_sum] += cnt;
                }
            }
        }

        /* Copy dp_next into dp_cur for the next iteration. */
        for (int r = 0; r < 9; ++r)
            for (int s = 0; s < 9; ++s)
                dp_cur[r][s] = dp_next[r][s];
    }

    /* Sum all possibilities to obtain the final count. */
    seq_t total = 0;
    for (int r = 0; r < 9; ++r)
        for (int s = 0; s < 9; ++s)
            total += dp_cur[r][s];

    return total;
}
