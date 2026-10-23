/*
 * A033507  Number of matchings in graph P_{4} X P_{n}.
 * https://oeis.org/A033507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of matchings in P_4 × P_n (OEIS A033507)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following uses a transfer‑matrix DP over the 4‑vertex column. */
#include <stdint.h>
#include <stdbool.h>

/* Transfer matrix: M[left][right] = number of ways to transition from
   state left to state right in one column.  States are 4‑bit masks. */
static uint8_t M[16][16];
static bool M_initialised = false;

/* Recursively enumerate all matchings inside a single column. */
static void dfs(int pos, int leftMask, int usedMask, int rightMask)
{
    if (pos == 4) {
        M[leftMask][rightMask] += 1;
        return;
    }
    if (usedMask & (1 << pos)) {
        dfs(pos + 1, leftMask, usedMask, rightMask);
        return;
    }
    /* leave the vertex unmatched */
    dfs(pos + 1, leftMask, usedMask, rightMask);
    /* match vertically with the next vertex if possible */
    if (pos + 1 < 4 && !(usedMask & (1 << (pos + 1)))) {
        dfs(pos + 2,
            leftMask,
            usedMask | (1 << pos) | (1 << (pos + 1)),
            rightMask);
    }
    /* match horizontally to the next column */
    dfs(pos + 1,
        leftMask,
        usedMask | (1 << pos),
        rightMask | (1 << pos));
}

/* Build the transfer matrix once. */
static void build_transfer_matrix(void)
{
    for (int left = 0; left < 16; ++left) {
        for (int j = 0; j < 16; ++j) {
            M[left][j] = 0;
        }
        dfs(0, left, left, 0);
    }
    M_initialised = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!M_initialised) {
        build_transfer_matrix();
    }

    seq_t dp_curr[16] = {0};
    seq_t dp_next[16];
    /* initial state: no pending horizontal edges before the first column */
    dp_curr[0] = (seq_t)1;

    for (idx_t col = 1; col <= n; ++col) {
        /* reset dp_next */
        for (int i = 0; i < 16; ++i) {
            dp_next[i] = (seq_t)0;
        }
        for (int i = 0; i < 16; ++i) {
            seq_t ways = dp_curr[i];
            if (ways == (seq_t)0) continue;
            for (int j = 0; j < 16; ++j) {
                uint8_t coeff = M[i][j];
                if (coeff == 0) continue;
                /* add ways * coeff (coeff is always small) */
                for (int k = 0; k < coeff; ++k) {
                    dp_next[j] += ways;
                }
            }
        }
        for (int i = 0; i < 16; ++i) {
            dp_curr[i] = dp_next[i];
        }
    }

    return dp_curr[0];
}
