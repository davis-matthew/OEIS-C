/*
 * A181215  Number of nX4 binary matrices with no three 1's adjacent in a line
 *          diagonally or antidiagonally.
 * https://oeis.org/A181215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices with no three 1’s adjacent
 *            in a diagonal or antidiagonal line. a(1)=16, a(2)=256; for n≥3
 *            it is computed by a DP that keeps the last two rows of the
 *            matrix
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (n == 1)
        return (seq_t)16;
    if (n == 2)
        return (seq_t)256;

    /* Transition table: allowed[old<<4 | mid][new] == true
       means that rows (old, mid, new) contain no forbidden
       diagonal or antidiagonal triple of 1’s. */
    static bool allowed[256][16];
    static bool init = false;
    if (!init) {
        for (int old = 0; old < 16; ++old) {
            for (int mid = 0; mid < 16; ++mid) {
                int s = (old << 4) | mid;
                for (int newR = 0; newR < 16; ++newR) {
                    bool ok = true;
                    /* diagonal triples: j = 0,1 */
                    for (int j = 0; j <= 1 && ok; ++j) {
                        int bo = (old >> j) & 1;
                        int bm = (mid >> (j + 1)) & 1;
                        int bn = (newR >> (j + 2)) & 1;
                        if (bo && bm && bn)
                            ok = false;
                    }
                    /* antidiagonal triples: j = 2,3 */
                    for (int j = 2; j <= 3 && ok; ++j) {
                        int bo = (old >> j) & 1;
                        int bm = (mid >> (j - 1)) & 1;
                        int bn = (newR >> (j - 2)) & 1;
                        if (bo && bm && bn)
                            ok = false;
                    }
                    allowed[s][newR] = ok;
                }
            }
        }
        init = true;
    }

    /* DP arrays: dp[s] counts sequences of current length that end
       with the last two rows represented by state s. */
    seq_t dp[256];
    seq_t ndp[256];

    /* Initial state: any pair of rows is allowed for length 2. */
    for (int i = 0; i < 256; ++i)
        dp[i] = (seq_t)1;

    /* Extend the matrix row by row. */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 256; ++i)
            ndp[i] = 0;

        for (int old = 0; old < 16; ++old) {
            for (int mid = 0; mid < 16; ++mid) {
                int s = (old << 4) | mid;
                seq_t cnt = dp[s];
                if (cnt == 0)
                    continue;
                for (int newR = 0; newR < 16; ++newR) {
                    if (allowed[s][newR]) {
                        int ns = (mid << 4) | newR;
                        ndp[ns] += cnt;
                    }
                }
            }
        }

        /* Copy new counts back into dp. */
        for (int i = 0; i < 256; ++i)
            dp[i] = ndp[i];
    }

    /* Sum over all ending states to obtain total number of matrices. */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += dp[i];

    return total;
}
