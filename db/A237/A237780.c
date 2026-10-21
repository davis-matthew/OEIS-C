/*
 * A237780  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237780
   Computation via dynamic programming over 2‑column arrays.
   Each 2×2 subblock has value
       f = max + upper_median + min – lower_median,
   where the four entries are sorted v0≤v1≤v2≤v3.
   Adjacent subblocks (vertically) must have |f difference| = 1.
   a(n) counts (n+1)×2 0..3 arrays satisfying this.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Undefined for n==0 – return 0 */
    if (n == 0) return (seq_t)0;

    /* Pre‑compute f for every pair of rows (0..15) */
    static unsigned char f_val[16][16];
    static bool f_ready = false;
    if (!f_ready) {
        for (int r1 = 0; r1 < 16; ++r1) {
            int a1 = r1 >> 2,   b1 = r1 & 3;
            for (int r2 = 0; r2 < 16; ++r2) {
                int a2 = r2 >> 2,   b2 = r2 & 3;
                int vals[4] = {a1, b1, a2, b2};
                /* sort vals: simple insertion sort */
                for (int i = 1; i < 4; ++i)
                    for (int j = i; j > 0 && vals[j-1] > vals[j]; --j) {
                        int t = vals[j]; vals[j] = vals[j-1]; vals[j-1] = t;
                    }
                /* v3 + v2 + v0 – v1 */
                f_val[r1][r2] = (unsigned char)(vals[3] + vals[2] + vals[0] - vals[1]);
            }
        }
        f_ready = true;
    }

    /* DP tables: cur[row][f] = #ways finishing with this row and last f */
    static seq_t cur[16][7];
    static seq_t nxt[16][7];

    /* initialise cur to 0 */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 7; ++j)
            cur[i][j] = 0;

    /* initial pair of rows: (r0,r1) */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1) {
            unsigned char f0 = f_val[r0][r1];
            cur[r1][f0] += 1;
        }

    /* add remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next table */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 7; ++j)
                nxt[i][j] = 0;

        for (int cur_row = 0; cur_row < 16; ++cur_row) {
            for (int fp = 0; fp < 7; ++fp) {
                seq_t ways = cur[cur_row][fp];
                if (ways == 0) continue;
                for (int next_row = 0; next_row < 16; ++next_row) {
                    unsigned char fcur = f_val[cur_row][next_row];
                    int diff = (fp > fcur) ? (fp - fcur) : (fcur - fp);
                    if (diff == 1) {
                        nxt[next_row][fcur] += ways;
                    }
                }
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 7; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum all endings */
    seq_t total = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 7; ++j)
            total += cur[i][j];

    return total;
}
