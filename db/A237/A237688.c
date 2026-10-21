/*
 * A237688  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{6} sum_{i_0,...,i_n} 1 * such that
 *            f(i_j,i_{j+1}) = k for all j * where i_j ∈ {0,…,15} represent
 *            2‑column rows * and f is max + upper‑median + min – lower‑median
 *            * of the corresponding 2×2 block
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int block_val[16][16];
static bool tbl_inited = false;

static void init_table(void) {
    if (tbl_inited) return;
    for (int r = 0; r < 16; ++r) {
        int a0 = r / 4;   /* column 0 of the top row  */
        int a1 = r % 4;   /* column 1 of the top row  */
        for (int s = 0; s < 16; ++s) {
            int b0 = s / 4;   /* column 0 of the bottom row */
            int b1 = s % 4;   /* column 1 of the bottom row */

            /* sort the four values to find a,b,c,d */
            int v[4] = { a0, a1, b0, b1 };
            /* simple insertion sort for 4 numbers */
            for (int ii = 1; ii < 4; ++ii) {
                int key = v[ii];
                int jj = ii - 1;
                while (jj >= 0 && v[jj] > key) {
                    v[jj + 1] = v[jj];
                    --jj;
                }
                v[jj + 1] = key;
            }
            int a = v[0], b = v[1], c = v[2], d = v[3];
            block_val[r][s] = d + c + a - b;   /* value in 0..6 */
        }
    }
    tbl_inited = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1 */
    init_table();

    seq_t answer = 0;
    for (int k = 0; k <= 6; ++k) {          /* constant value of all blocks */
        seq_t dp_prev[16], dp_cur[16];
        /* any first row is allowed */
        for (int i = 0; i < 16; ++i) dp_prev[i] = 1;

        for (idx_t step = 0; step < n; ++step) {  /* n transitions (n+1 rows) */
            for (int j = 0; j < 16; ++j) dp_cur[j] = 0;
            for (int i = 0; i < 16; ++i) {
                seq_t vi = dp_prev[i];
                if (!vi) continue;            /* speed up for zero counts */
                for (int j = 0; j < 16; ++j) {
                    if (block_val[i][j] == k)
                        dp_cur[j] += vi;
                }
            }
            /* move to next step */
            for (int j = 0; j < 16; ++j) dp_prev[j] = dp_cur[j];
        }

        /* sum all ending rows */
        for (int j = 0; j < 16; ++j)
            answer += dp_prev[j];
    }
    return answer;
}
