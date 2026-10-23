/*
 * A189618  Number of 3Xn binary arrays without the pattern 0 1 0 diagonally,
 *          antidiagonally or horizontally.
 * https://oeis.org/A189618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n binary arrays without the pattern 0 1 0
 *            diagonally, antidiagonally or horizontally
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the offset, undefined */
    if (n == 1) return 8;          /* 2^3 */
    if (n == 2) return 64;         /* 2^3 × 2^3 */

    /* Precompute validity of adding a column to a pair of previous columns. */
    static bool allowed[8][8][8];
    static bool initialized = false;
    if (!initialized) {
        for (int p0 = 0; p0 < 8; ++p0) {
            for (int p1 = 0; p1 < 8; ++p1) {
                for (int p2 = 0; p2 < 8; ++p2) {
                    bool ok = true;

                    /* horizontal pattern in each of the 3 rows */
                    for (int r = 0; r < 3 && ok; ++r) {
                        int a = (p0 >> r) & 1;
                        int b = (p1 >> r) & 1;
                        int c = (p2 >> r) & 1;
                        if (a == 0 && b == 1 && c == 0) ok = false;
                    }
                    /* down‑right diagonal 0‑1‑0 of cells (1,p0), (2,p1), (3,p2) */
                    if (ok) {
                        int a = (p0 >> 0) & 1;
                        int b = (p1 >> 1) & 1;
                        int c = (p2 >> 2) & 1;
                        if (a == 0 && b == 1 && c == 0) ok = false;
                    }
                    /* up‑right diagonal 0‑1‑0 of cells (3,p0), (2,p1), (1,p2) */
                    if (ok) {
                        int a = (p0 >> 2) & 1;
                        int b = (p1 >> 1) & 1;
                        int c = (p2 >> 0) & 1;
                        if (a == 0 && b == 1 && c == 0) ok = false;
                    }

                    allowed[p0][p1][p2] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP over the last two columns.
       dp[p0*8 + p1] holds the number of ways to reach the current
       column with previous columns (p0, p1). */
    seq_t dp[64];
    seq_t next[64];

    /* Initial state for n == 2: all 64 column pairs are possible */
    for (int i = 0; i < 64; ++i) dp[i] = 1;

    for (idx_t col = 3; col <= n; ++col) {
        /* Reset next state */
        for (int i = 0; i < 64; ++i) next[i] = 0;

        for (int p0 = 0; p0 < 8; ++p0) {
            for (int p1 = 0; p1 < 8; ++p1) {
                int pos = p0 * 8 + p1;
                seq_t val = dp[pos];
                if (val == 0) continue;
                for (int p2 = 0; p2 < 8; ++p2) {
                    if (!allowed[p0][p1][p2]) continue;
                    int npos = p1 * 8 + p2;
                    next[npos] += val;
                }
            }
        }

        /* Move to next position */
        for (int i = 0; i < 64; ++i) dp[i] = next[i];
    }

    /* Sum all sequences of length n */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i) total += dp[i];
    return total;
}
