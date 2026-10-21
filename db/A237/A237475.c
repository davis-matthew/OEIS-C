/*
 * A237475  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over row states. Two
 *            consecutive rows are allowed to transition only when both 2×2
 *            subblocks (columns 0–1 and 1–2) have the same sum of maximum and
 *            lower median. For each of the 5 possible sums (0…4) the number
 *            of sequences of n+1 rows is the sum of all ent...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static unsigned char allowed[5][27][27];
static bool initialized = false;

static void init_allowed(void)
{
    static int row[27][3];
    for (int state = 0; state < 27; ++state) {
        int v = state;
        for (int col = 0; col < 3; ++col) {
            row[state][col] = v % 3;
            v /= 3;
        }
    }

    for (int s = 0; s < 5; ++s)
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                allowed[s][i][j] = 0;

    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 27; ++j) {
            /* left block */
            int a[4] = { row[i][0], row[i][1], row[j][0], row[j][1] };
            for (int k = 0; k < 4; ++k)
                for (int l = k + 1; l < 4; ++l)
                    if (a[l] < a[k]) {
                        int t = a[k]; a[k] = a[l]; a[l] = t;
                    }
            int s_left = a[1] + a[3];

            /* right block */
            int b[4] = { row[i][1], row[i][2], row[j][1], row[j][2] };
            for (int k = 0; k < 4; ++k)
                for (int l = k + 1; l < 4; ++l)
                    if (b[l] < b[k]) {
                        int t = b[k]; b[k] = b[l]; b[l] = t;
                    }
            int s_right = b[1] + b[3];

            if (s_left == s_right) {
                allowed[s_left][i][j] = 1;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_allowed();

    seq_t total = 0;

    for (int S = 0; S < 5; ++S) {
        seq_t dp[27];
        seq_t ndp[27];

        for (int i = 0; i < 27; ++i) dp[i] = 1;

        for (idx_t step = 0; step < n; ++step) {
            for (int j = 0; j < 27; ++j) ndp[j] = 0;

            for (int i = 0; i < 27; ++i) {
                if (!dp[i]) continue;
                for (int j = 0; j < 27; ++j) {
                    if (allowed[S][i][j]) ndp[j] += dp[i];
                }
            }
            for (int j = 0; j < 27; ++j) dp[j] = ndp[j];
        }

        seq_t sumS = 0;
        for (int i = 0; i < 27; ++i) sumS += dp[i];
        total += sumS;
    }

    return total;
}
