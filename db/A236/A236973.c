/*
 * A236973  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 0..3 arrays for which (max +
 *            upper_median – min) is equal for every 2×2 subblock
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    enum { ROWS = 16, LOG_MAX = 16, FMAX = 7 };
    static bool initialized = false;
    static seq_t M_pow[FMAX][LOG_MAX][ROWS][ROWS];

    if (!initialized) {
        /* Build adjacency matrices for each possible value of f (0..6). */
        for (int i = 0; i < ROWS; ++i) {
            int a = i >> 2;
            int b = i & 3;
            for (int j = 0; j < ROWS; ++j) {
                int c = j >> 2;
                int d = j & 3;

                int v[4] = { a, b, c, d };
                int minv = 4, max1 = -1, max2 = -1;
                for (int t = 0; t < 4; ++t) {
                    int val = v[t];
                    if (val < minv) minv = val;
                    if (val > max1) { max2 = max1; max1 = val; }
                    else if (val > max2) { max2 = val; }
                }
                int f = max1 + max2 - minv;   /* 0 <= f <= 6 */
                M_pow[f][0][i][j] = 1;
                M_pow[f][0][j][i] = 1;        /* symmetric */
            }
        }

        /* Pre‑compute powers of each adjacency matrix. */
        for (int f = 0; f < FMAX; ++f) {
            for (int k = 1; k < LOG_MAX; ++k) {
                for (int i = 0; i < ROWS; ++i) {
                    for (int j = 0; j < ROWS; ++j) {
                        seq_t sum = 0;
                        for (int m = 0; m < ROWS; ++m) {
                            sum += M_pow[f][k-1][i][m] * M_pow[f][k-1][m][j];
                        }
                        M_pow[f][k][i][j] = sum;
                    }
                }
            }
        }

        initialized = true;
    }

    if (n == 0) return 0;   /* undefined for n < 1 */

    seq_t result = 0;

    /* For each possible f, count walks of length n in its adjacency graph. */
    for (int f = 0; f < FMAX; ++f) {
        seq_t vec[ROWS];
        for (int i = 0; i < ROWS; ++i) vec[i] = 1;   /* start with all rows */

        idx_t temp = n;
        int maxbit = 0;
        while (temp) { ++maxbit; temp >>= 1; }

        for (int k = 0; k < maxbit; ++k) {
            if ((n >> k) & 1) {
                seq_t next[ROWS] = { 0 };
                for (int i = 0; i < ROWS; ++i) {
                    seq_t vi = vec[i];
                    if (vi == 0) continue;
                    for (int j = 0; j < ROWS; ++j) {
                        seq_t m = M_pow[f][k][i][j];
                        if (m == 0) continue;
                        next[j] += vi * m;
                    }
                }
                for (int i = 0; i < ROWS; ++i) vec[i] = next[i];
            }
        }

        seq_t sum = 0;
        for (int i = 0; i < ROWS; ++i) sum += vec[i];
        result += sum;
    }

    return result;
}
