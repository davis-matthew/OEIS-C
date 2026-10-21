/*
 * A233629  Number of (n+1)X(2+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 6.
 * https://oeis.org/A233629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays of entries 0..3 such that every
 *            2×2 block has sum of absolute edge differences equal to 6.
 *            Computed by a dynamic programming over the 64 possible 3‑column
 *            rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Transition table: for each row i, list of rows j that can follow. */
    static int next_len[64];
    static int next_list[64][64];
    static int init_done = 0;

    if (!init_done) {
        for (int i = 0; i < 64; ++i) {
            int a_val = (i >> 4) & 3;
            int b_val = (i >> 2) & 3;
            int c_val =  i        & 3;
            int cnt = 0;
            for (int j = 0; j < 64; ++j) {
                int d_val = (j >> 4) & 3;
                int e_val = (j >> 2) & 3;
                int f_val =  j        & 3;
                int sum1 = abs(a_val - b_val) + abs(b_val - e_val)
                         + abs(d_val - e_val) + abs(a_val - d_val);
                int sum2 = abs(b_val - c_val) + abs(c_val - f_val)
                         + abs(e_val - f_val) + abs(b_val - e_val);
                if (sum1 == 6 && sum2 == 6)
                    next_list[i][cnt++] = j;
            }
            next_len[i] = cnt;
        }
        init_done = 1;
    }

    /* DP arrays */
    seq_t dp_cur[64], dp_next[64];
    for (int i = 0; i < 64; ++i)
        dp_cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* reset next array */
        for (int i = 0; i < 64; ++i)
            dp_next[i] = (seq_t)0;
        /* propagate */
        for (int i = 0; i < 64; ++i) {
            seq_t val = dp_cur[i];
            int len = next_len[i];
            for (int k = 0; k < len; ++k) {
                int j = next_list[i][k];
                dp_next[j] = dp_next[j] + val;
            }
        }
        /* copy next to current for next iteration */
        for (int i = 0; i < 64; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum all possible last rows */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 64; ++i)
        result = result + dp_cur[i];
    return result;
}
