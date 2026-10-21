/*
 * A079926  Solution to the Dancing School Problem with n girls and n+7 boys:
 *          f(n,7).
 * https://oeis.org/A079926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n×(n+7) (0,1)-matrix B with B[i][j]=1
 *            iff i ≤ j ≤ i+7; computed by dynamic programming over a 7‑bit
 *            state
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* number of states: 2^7 = 128 */
    enum { STATE = 128 };

    seq_t cur[STATE];
    seq_t nxt[STATE];

    for (int m = 0; m < STATE; ++m) cur[m] = 0;
    cur[0] = 1;                     /* no columns used yet before row 1 */

    for (idx_t row = 0; row < n; ++row) {
        for (int m = 0; m < STATE; ++m) nxt[m] = 0;

        for (int mask = 0; mask < STATE; ++mask) {
            seq_t val = cur[mask];
            if (val == 0) continue;

            /* try to put a rook in one of the 8 allowed columns
               relative index r: 0→column row+1, 1→row+2, …, 7→row+8 */
            for (int r = 0; r < 8; ++r) {
                /* skip if column already used (bits 0..6 correspond to
                   row+1 .. row+7) */
                if (r <= 6 && ((mask >> r) & 1)) continue;

                int newMask = mask >> 1;           /* shift window */
                if (r == 0) {
                    /* column row+1 is outside the new window; nothing to set */
                } else if (r <= 6) {
                    newMask |= 1 << (r - 1);       /* set bit for column row+r */
                } else { /* r == 7, new column row+8 */
                    newMask |= 1 << 6;            /* bit 6 corresponds to new col */
                }
                nxt[newMask] += val;
            }
        }
        /* copy nxt to cur for next iteration */
        for (int m = 0; m < STATE; ++m) cur[m] = nxt[m];
    }

    /* sum over all final states – remaining columns are irrelevant */
    seq_t ans = 0;
    for (int m = 0; m < STATE; ++m) ans += cur[m];
    return ans;
}
