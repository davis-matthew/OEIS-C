/*
 * A197103  Half the number of nX2 0..3 arrays with no element equal to the
 *          average of its horizontal and vertical neighbors.
 * https://oeis.org/A197103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 0..3 n×2 arrays where no element equals the average
 *            of its horizontal and vertical neighbors; a(n) equals half that
 *            count. The routine uses dynamic programming over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n = 1 has 12 valid arrays; return half. */
    if (n <= 1) return (seq_t)6;

    /* Pre‑compute validity tables once. */
    static bool good_top[16][16];
    static bool good_middle[16][16][16];
    static bool init = false;

    if (!init) {
        /* Constraints between first two rows (top row). */
        for (int r0 = 0; r0 < 16; ++r0) {
            int c00 = r0 & 3;
            int c01 = (r0 >> 2) & 3;
            for (int r1 = 0; r1 < 16; ++r1) {
                int c10 = r1 & 3;
                int c11 = (r1 >> 2) & 3;
                bool ok = true;
                if (2 * c00 == c01 + c10) ok = false;         /* left cell */
                if (2 * c01 == c00 + c11) ok = false;         /* right cell */
                good_top[r0][r1] = ok;
            }
        }

        /* Constraints for each interior cell involving
           the row above, the current row, and the row below. */
        for (int rprev = 0; rprev < 16; ++rprev) {
            int a0 = rprev & 3;             /* top‑left   */
            int a1 = (rprev >> 2) & 3;      /* top‑right  */
            for (int rcurr = 0; rcurr < 16; ++rcurr) {
                int b0 = rcurr & 3;          /* mid‑left   */
                int b1 = (rcurr >> 2) & 3;   /* mid‑right  */
                for (int rnext = 0; rnext < 16; ++rnext) {
                    int c0 = rnext & 3;      /* bot‑left   */
                    int c1 = (rnext >> 2) & 3;/* bot‑right  */
                    bool ok = true;
                    if (3 * b0 == b1 + a0 + c0) ok = false;   /* left cell */
                    if (3 * b1 == b0 + a1 + c1) ok = false;   /* right cell */
                    good_middle[rprev][rcurr][rnext] = ok;
                }
            }
        }
        init = true;
    }

    /* Dynamic programming: dp[prev][curr] = ways up to current row. */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* First two rows – only top constraints apply. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = good_top[i][j] ? (seq_t)1 : (seq_t)0;

    /* Process rows 3..n (indices 2..n-1). */
    for (idx_t r = 2; r < n; ++r) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == (seq_t)0) continue;
                for (int next = 0; next < 16; ++next) {
                    if (!good_middle[prev][curr][next]) continue;
                    ndp[curr][next] += ways;
                }
            }
        }

        /* Swap dp and ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Bottom row constraints – symmetric to top. */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr) {
            if (!good_top[curr][prev]) continue;   /* bottom check */
            total += dp[prev][curr];
        }

    return total / (seq_t)2;
}
