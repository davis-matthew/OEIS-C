/*
 * A237405  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          upper median plus the minimum of every 2 X 2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming over rows. 
   The procedure counts 0–2 arrays of size (n+1)×2 that satisfy the
   “sum of the minimum, upper median and maximum of each 2×2 block”
   differing by exactly one for vertically adjacent blocks. */
#include <stdbool.h>

static const int ROW[9][2] = {
    {0,0},{0,1},{0,2},
    {1,0},{1,1},{1,2},
    {2,0},{2,1},{2,2}
};

/* compute the sum for a pair of rows */
static unsigned char row_pair_sum(int a, int b)
{
    int v[4];
    v[0] = ROW[a][0];
    v[1] = ROW[a][1];
    v[2] = ROW[b][0];
    v[3] = ROW[b][1];
    /* simple insertion sort on 4 elements */
    for (int i = 1; i < 4; ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
    /* min + upper median + max */
    return (unsigned char)(v[0] + v[2] + v[3]);
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Pre‑compute pair sums once */
    static bool init = false;
    static unsigned char pair_sum[9][9];
    if (!init) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                pair_sum[i][j] = row_pair_sum(i, j);
        init = true;
    }

    /* DP arrays: dp[row][sum] */
    static seq_t cur[9][7];
    static seq_t nxt[9][7];

    /* initialize cur to zero */
    for (int r = 0; r < 9; ++r)
        for (int s = 0; s < 7; ++s)
            cur[r][s] = 0;

    /* build initial pairs (rows 0 and 1) */
    for (int r0 = 0; r0 < 9; ++r0) {
        for (int r1 = 0; r1 < 9; ++r1) {
            unsigned char sum = pair_sum[r0][r1];
            cur[r1][sum] += 1;
        }
    }

    /* for n == 1, we are done (only one 2×2 block) */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 9; ++r)
            for (int s = 0; s < 7; ++s)
                total += cur[r][s];
        return total;
    }

    /* iterate for rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero nxt */
        for (int r = 0; r < 9; ++r)
            for (int s = 0; s < 7; ++s)
                nxt[r][s] = 0;

        for (int prevr = 0; prevr < 9; ++prevr) {
            for (int prevS = 0; prevS < 7; ++prevS) {
                seq_t ways = cur[prevr][prevS];
                if (ways == 0) continue;
                for (int nextr = 0; nextr < 9; ++nextr) {
                    unsigned char newS = pair_sum[prevr][nextr];
                    int diff = (prevS > newS) ? (prevS - newS) : (newS - prevS);
                    if (diff == 1)
                        nxt[nextr][newS] += ways;
                }
            }
        }
        /* swap cur and nxt for next iteration */
        for (int r = 0; r < 9; ++r)
            for (int s = 0; s < 7; ++s)
                cur[r][s] = nxt[r][s];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int r = 0; r < 9; ++r)
        for (int s = 0; s < 7; ++s)
            total += cur[r][s];
    return total;
}
