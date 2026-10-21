/*
 * A236813  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236813
   a(n) = number of (n+1) × 3 0..2 arrays such that the value
          max + lower median – min of every 2×2 subblock is the same
          for all subblocks.
   A DP is performed for each possible common value v ∈ {0,1,2,3,4} */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n must be ≥ 1, but return 0 if called with n==0 */
    if (n == 0) return (seq_t)0;

    /* Static tables are constructed once */
    static bool built = false;
    /* encode a row (length‑3, digits 0–2) as three base‑3 digits */
    static unsigned char row_digits[27][3];
    /* allowed[v][i][j] == 1 ⇔ rows i and j form a 2‑row block
       whose two 2×2 subblocks both yield the value v */
    static unsigned char allowed[5][27][27];

    if (!built) {
        /* Pre‑compute row digits */
        for (int i = 0; i < 27; ++i) {
            int x = i;
            row_digits[i][0] = x % 3;
            row_digits[i][1] = (x / 3) % 3;
            row_digits[i][2] = (x / 9) % 3;
        }

        /* Evaluate every pair of rows */
        for (int i = 0; i < 27; ++i) {
            for (int j = 0; j < 27; ++j) {
                int d[3] = {row_digits[i][0], row_digits[i][1], row_digits[i][2]};
                int e[3] = {row_digits[j][0], row_digits[j][1], row_digits[j][2]};

                /* two 2×2 blocks: columns 0–1 and 1–2 */
                int block[2][4] = {
                    {d[0], d[1], e[0], e[1]},
                    {d[1], d[2], e[1], e[2]}
                };
                int val[2];
                for (int b = 0; b < 2; ++b) {
                    /* sort block[b] to obtain min, two medians, max */
                    for (int a = 0; a < 4; ++a)
                        for (int c = a + 1; c < 4; ++c)
                            if (block[b][a] > block[b][c]) {
                                int t = block[b][a];
                                block[b][a] = block[b][c];
                                block[b][c] = t;
                            }
                    /* max + lower median – min */
                    val[b] = block[b][3] + block[b][1] - block[b][0];
                }
                if (val[0] == val[1] && val[0] <= 4) {
                    allowed[val[0]][i][j] = 1;
                }
            }
        }
        built = true;
    }

    seq_t total = (seq_t)0;

    /* For each admissible common value v, count walks of length n
       in the directed graph of rows with edges indicated by allowed[v] */
    for (int v = 0; v < 5; ++v) {
        seq_t dp_prev[27], dp_curr[27];

        /* start with any first row */
        for (int i = 0; i < 27; ++i) dp_prev[i] = (seq_t)1;

        /* perform n transitions (there are n+1 rows) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 27; ++i) dp_curr[i] = (seq_t)0;

            for (int i = 0; i < 27; ++i) {
                seq_t vi = dp_prev[i];
                if (vi == (seq_t)0) continue;

                for (int j = 0; j < 27; ++j) {
                    if (allowed[v][i][j]) {
                        dp_curr[j] = dp_curr[j] + vi;
                    }
                }
            }
            /* copy back for next step */
            for (int i = 0; i < 27; ++i) dp_prev[i] = dp_curr[i];
        }

        /* sum over all possible last rows */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 27; ++i) sum = sum + dp_prev[i];
        total = total + sum;
    }

    return total;
}
