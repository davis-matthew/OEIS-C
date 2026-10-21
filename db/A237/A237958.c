/*
 * A237958  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of (n+1)×2 arrays with entries 0..2 such that
 *            * for every adjacent pair of 2×2 subblocks their values * f =
 *            max + lower_median - min differ by exactly one
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* All possible rows of two cells (9 possibilities). */
static const int row_vals[9][2] = {
    {0,0},{0,1},{0,2},
    {1,0},{1,1},{1,2},
    {2,0},{2,1},{2,2}
};

/* f matrix for every pair of rows (precomputed once). */
static int fmat[9][9];
static int fmat_inited = 0;
static void init_fmat(void)
{
    if (fmat_inited) return;
    for (int a = 0; a < 9; ++a) {
        for (int b = 0; b < 9; ++b) {
            int v[4] = { row_vals[a][0], row_vals[a][1],
                         row_vals[b][0], row_vals[b][1] };
            int min = v[0], max = v[0];
            for (int k = 1; k < 4; ++k) {
                if (v[k] < min) min = v[k];
                if (v[k] > max) max = v[k];
            }
            /* sort the four values to get the lower median (second element). */
            int arr[4] = { v[0], v[1], v[2], v[3] };
            for (int i = 0; i < 3; ++i)
                for (int j = i + 1; j < 4; ++j)
                    if (arr[j] < arr[i]) {
                        int tmp = arr[i];
                        arr[i] = arr[j];
                        arr[j] = tmp;
                    }
            int lower_median = arr[1];
            fmat[a][b] = max + lower_median - min;   /* values 0..4 */
        }
    }
    fmat_inited = 1;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for n=0 */

    init_fmat();

    /* dp[row][f] holds number of ways to reach the current last row
     * with the last subblock having value f. */
    seq_t dp[9][5] = {0};
    seq_t next_dp[9][5];

    /* Step 2: after processing two rows (one subblock). */
    for (int prev = 0; prev < 9; ++prev) {
        for (int cur = 0; cur < 9; ++cur) {
            int f = fmat[prev][cur];
            dp[cur][f] += (seq_t)1;
        }
    }

    if (n == 1) {
        seq_t sum = 0;
        for (int r = 0; r < 9; ++r)
            for (int f = 0; f < 5; ++f)
                sum += dp[r][f];
        return sum;
    }

    /* For n >= 2, we need to add rows up to n+1. */
    for (idx_t i = 3; i <= n + 1; ++i) {
        /* clear next_dp */
        for (int r = 0; r < 9; ++r)
            for (int f = 0; f < 5; ++f)
                next_dp[r][f] = 0;

        for (int prev = 0; prev < 9; ++prev) {
            for (int fprev = 0; fprev < 5; ++fprev) {
                seq_t val = dp[prev][fprev];
                if (val == 0) continue;
                for (int cur = 0; cur < 9; ++cur) {
                    int fcurr = fmat[prev][cur];
                    int diff = fcurr - fprev;
                    if (diff == 1 || diff == -1) {
                        next_dp[cur][fcurr] += val;
                    }
                }
            }
        }

        /* copy next_dp back to dp for next iteration */
        for (int r = 0; r < 9; ++r)
            for (int f = 0; f < 5; ++f)
                dp[r][f] = next_dp[r][f];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int r = 0; r < 9; ++r)
        for (int f = 0; f < 5; ++f)
            result += dp[r][f];
    return result;
}
