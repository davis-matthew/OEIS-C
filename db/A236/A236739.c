/*
 * A236739  Number of (n+1) X (1+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A236739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×2 arrays over {0,1,2} such that the
 *            * upper median plus the lower median of every 2×2 subblock is
 *            the same * for all subblocks. It is computed with a
 *            transfer‑matrix DP on the * 9 possible rows
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* All 3^2 = 9 possible rows of length 2 over {0,1,2}. */
    static const int row[9][2] = {
        {0,0},{0,1},{0,2},
        {1,0},{1,1},{1,2},
        {2,0},{2,1},{2,2}
    };

    /* med[i][j] = v2+v3 of the 2×2 block formed by rows i (top) and j (bottom). */
    static int med[9][9];
    static bool ready = false;
    if (!ready) {
        for (int i = 0; i < 9; ++i) {
            int a = row[i][0], b = row[i][1];
            for (int j = 0; j < 9; ++j) {
                int c = row[j][0], d = row[j][1];
                int v[4] = {a, b, c, d};
                /* sort the 4 values to obtain the middle two */
                for (int p = 0; p < 4; ++p)
                    for (int q = p + 1; q < 4; ++q)
                        if (v[p] > v[q]) {
                            int t = v[p]; v[p] = v[q]; v[q] = t;
                        }
                med[i][j] = v[1] + v[2];
            }
        }
        ready = true;
    }

    /* counts[s][r] = number of sequences of current length that end on row r
     * and have constant median sum s (0…4). */
    seq_t counts[5][9] = {0};
    seq_t next[5][9];

    /* initialize for length 2 (one subblock). */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            ++counts[med[i][j]][j];

    /* extend to length n+1. */
    for (idx_t step = 2; step <= n; ++step) {
        for (int s = 0; s < 5; ++s)
            for (int r = 0; r < 9; ++r)
                next[s][r] = 0;

        for (int s = 0; s < 5; ++s) {
            for (int i = 0; i < 9; ++i) {
                seq_t cur = counts[s][i];
                if (!cur) continue;
                for (int j = 0; j < 9; ++j) {
                    if (med[i][j] == s)
                        next[s][j] += cur;
                }
            }
        }

        for (int s = 0; s < 5; ++s)
            for (int r = 0; r < 9; ++r)
                counts[s][r] = next[s][r];
    }

    seq_t total = 0;
    for (int s = 0; s < 5; ++s)
        for (int r = 0; r < 9; ++r)
            total += counts[s][r];

    return total;
}
