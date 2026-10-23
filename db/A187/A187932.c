/*
 * A187932  Number of (n+2)X3 binary arrays with every 2X2 subblock sum equal
 *          to some diagonal or antidiagonal neighbor 2X2 subblock sum.
 * https://oeis.org/A187932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A187932 by dynamic programming over rows.
   The program counts (n+2)×3 binary arrays where for each 2×2 subblock
   the sum equals the sum of its down‑right or down‑left diagonal neighbour.
   The DP propagates satisfaction of the left/right blocks from one pair
   of adjacent rows to the next. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) is defined for the offset 1; for n<1 return 0. */
    if (n < 1) return 0;

    /* Pre‑compute 2×2 block sums for all pairs of 3‑bit rows. */
    int sumsL[8][8];   /* left block sums (columns 0‑1) */
    int sumsR[8][8];   /* right block sums (columns 1‑2) */

    for (int r1 = 0; r1 < 8; ++r1) {
        for (int r2 = 0; r2 < 8; ++r2) {
            int l = ((r1 >> 0) & 1) + ((r1 >> 1) & 1) +
                    ((r2 >> 0) & 1) + ((r2 >> 1) & 1);
            int r = ((r1 >> 1) & 1) + ((r1 >> 2) & 1) +
                    ((r2 >> 1) & 1) + ((r2 >> 2) & 1);
            sumsL[r1][r2] = l;
            sumsR[r1][r2] = r;
        }
    }

    /* DP arrays.  dp[prevRow][curRow][mask] holds the number of ways
       to build rows up to curRow, with 'mask' bits indicating which of
       the blocks of the pair (prevRow,curRow) have already been satisfied
       by the preceding pair.  Mask bit 0 = left block, bit 1 = right block. */
    seq_t dp[8][8][4] = {};        /* initialised to zero */
    seq_t dp_next[8][8][4];

    /* Initialise with the first pair of rows (row 0 and row 1). */
    for (int r0 = 0; r0 < 8; ++r0)
        for (int r1 = 0; r1 < 8; ++r1)
            dp[r0][r1][0] = 1;

    /* Iterate over the remaining rows (row 2 … row n+1). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero dp_next by explicit assignment (do not use memset for non-POD types). */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int m = 0; m < 4; ++m)
                    dp_next[i][j][m] = 0;

        for (int r_prev = 0; r_prev < 8; ++r_prev) {
            for (int r_cur = 0; r_cur < 8; ++r_cur) {
                for (int mask = 0; mask < 4; ++mask) {
                    seq_t cnt = dp[r_prev][r_cur][mask];
                    if (cnt == 0) continue;

                    /* Try all possible next rows. */
                    for (int r_next = 0; r_next < 8; ++r_next) {
                        /* Sums for the pair (r_prev, r_cur). */
                        int L_prev = sumsL[r_prev][r_cur];
                        int R_prev = sumsR[r_prev][r_cur];

                        /* Sums for the pair (r_cur, r_next). */
                        int L_cur  = sumsL[r_cur][r_next];
                        int R_cur  = sumsR[r_cur][r_next];

                        /* For the previous pair to be fully satisfied,
                           its left/right blocks must equal the corresponding
                           right/left blocks of the current pair. */
                        if ((mask & 1) == 0 && L_prev != R_cur) continue;
                        if ((mask & 2) == 0 && R_prev != L_cur) continue;

                        /* Determine which blocks of the current pair have
                           been satisfied by the previous pair. */
                        int newMask = 0;
                        if (L_cur == R_prev) newMask |= 1;   /* left block satisfied */
                        if (R_cur == L_prev) newMask |= 2;   /* right block satisfied */

                        dp_next[r_cur][r_next][newMask] += cnt;
                    }
                }
            }
        }

        /* Copy dp_next into dp for the next iteration (element-wise assignment). */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int m = 0; m < 4; ++m)
                    dp[i][j][m] = dp_next[i][j][m];
    }

    /* After processing all rows, only states where the last pair is
       fully satisfied (mask==3) correspond to valid arrays. */
    seq_t result = 0;
    for (int r_prev = 0; r_prev < 8; ++r_prev)
        for (int r_cur = 0; r_cur < 8; ++r_cur)
            result += dp[r_prev][r_cur][3];

    return result;
}
