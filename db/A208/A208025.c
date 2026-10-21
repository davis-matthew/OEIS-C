/*
 * A208025  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A208025: number of n×5 0..1 arrays avoiding
   horizontally 000 and 001, and vertically 000 and 101.
   Computation by dynamic programming over 5‑bit row masks.
*/

#include <stdint.h>

#define MAXMASK 32   /* 5‑bit rows → 2^5 = 32 possible masks */

seq_t a(idx_t n)
{
    /* -----  Structures that are computed once  ----- */
    static int      allowedCount = 0;
    static int      idxToMask[MAXMASK];
    static uint32_t transition[MAXMASK][MAXMASK];
    static int      initialized = 0;

    if (!initialized) {
        /* ---- collect masks that satisfy horizontal rule ----- */
        for (int m = 0; m < MAXMASK; ++m) {
            int ok = 1;
            for (int j = 0; j < 3; ++j) {
                int b0 = (m >> j) & 1;
                int b1 = (m >> (j + 1)) & 1;
                int b2 = (m >> (j + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                    (b0 == 0 && b1 == 0 && b2 == 1)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                idxToMask[allowedCount] = m;
                ++allowedCount;
            }
        }

        /* ---- pre‑compute vertical compatibility ----- */
        for (int i = 0; i < allowedCount; ++i) {
            for (int j = 0; j < allowedCount; ++j) {
                uint32_t mask = 0;
                for (int k = 0; k < allowedCount; ++k) {
                    int ok = 1;
                    for (int col = 0; col < 5; ++col) {
                        int top    = (idxToMask[i] >> col) & 1;
                        int middle = (idxToMask[j] >> col) & 1;
                        int bottom = (idxToMask[k] >> col) & 1;
                        if ((top == 0 && middle == 0 && bottom == 0) ||
                            (top == 1 && middle == 0 && bottom == 1)) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok)
                        mask |= (1U << k);
                }
                transition[i][j] = mask;
            }
        }
        initialized = 1;
    }

    /* ----- trivial offsets ----- */
    if (n == 0) return 0; /* unused, offset is 1 */

    if (n == 1) return (seq_t)allowedCount;           /* 16 */

    if (n == 2) return (seq_t)allowedCount * (seq_t)allowedCount; /* 256 */

    /* ----- DP for rows 3 … n ----- */
    seq_t dp[MAXMASK][MAXMASK] = {0};
    seq_t newdp[MAXMASK][MAXMASK] = {0};

    /* initial pair of rows: any allowed pair is valid */
    for (int i = 0; i < allowedCount; ++i)
        for (int j = 0; j < allowedCount; ++j)
            dp[i][j] = 1;

    for (idx_t row = 3; row <= n; ++row) {
        /* reset newdp */
        for (int i = 0; i < allowedCount; ++i)
            for (int j = 0; j < allowedCount; ++j)
                newdp[i][j] = 0;

        /* extend each pair (i,j) by an allowed third row k */
        for (int i = 0; i < allowedCount; ++i) {
            for (int j = 0; j < allowedCount; ++j) {
                seq_t val = dp[i][j];
                if (!val) continue;
                uint32_t mask = transition[i][j];
                for (int k = 0; k < allowedCount; ++k) {
                    if (mask & (1U << k))
                        newdp[j][k] += val;
                }
            }
        }
        /* prepare for next iteration */
        for (int i = 0; i < allowedCount; ++i)
            for (int j = 0; j < allowedCount; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* sum all configurations for the final two rows */
    seq_t total = 0;
    for (int i = 0; i < allowedCount; ++i)
        for (int j = 0; j < allowedCount; ++j)
            total += dp[i][j];

    return total;
}
