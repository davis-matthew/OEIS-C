/*
 * A237484  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A237484
   Count of (n+1)×4 0..3 arrays where
   the value max + min − lower median is equal for all 2×2 subblocks. */

#include <stdint.h>

/* small helper to compute the value for one 2×2 block */
static inline int block_value(int a0, int a1, int b0, int b1)
{
    int v[4] = {a0, a1, b0, b1};
    /* simple four‑element sort */
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (v[j] < v[i]) {
                int t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
    int mn = v[0], med = v[1], mx = v[3];
    return mx + mn - med;
}

/* ----------------------------------------------------------
   The main function.  It builds the transition tables once
   and then counts the arrays via dynamic programming
   for each possible constant value (0–3).
   ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static unsigned char digits[256][4];
    /* adjacency information */
    static uint16_t deg[4][256];               /* number of neighbours per row per value */
    static uint16_t neigh[4][256][256];        /* neighbour lists (fixed size) */

    if (!initialized) {
        /* pre‑compute the 4 digits for each row value 0..255 */
        for (int r = 0; r < 256; ++r)
            for (int col = 0; col < 4; ++col)
                digits[r][col] = (unsigned char)((r >> (2 * col)) & 3);

        /* build edge lists */
        for (int e = 0; e < 4; ++e)
            for (int r = 0; r < 256; ++r)
                deg[e][r] = 0;

        for (int r = 0; r < 256; ++r) {
            for (int rp = 0; rp < 256; ++rp) {
                int e0 = block_value(digits[r][0], digits[r][1], digits[rp][0], digits[rp][1]);
                int e1 = block_value(digits[r][1], digits[r][2], digits[rp][1], digits[rp][2]);
                int e2 = block_value(digits[r][2], digits[r][3], digits[rp][2], digits[rp][3]);
                if (e0 == e1 && e1 == e2) {
                    uint16_t pos = deg[e0][r];
                    neigh[e0][r][pos] = (uint16_t)rp;
                    deg[e0][r] = pos + 1;
                }
            }
        }
        initialized = 1;
    }

    seq_t total = 0;

    /* iterate over the 4 possible constant values */
    for (int e = 0; e < 4; ++e) {
        seq_t dp_curr[256];
        seq_t dp_next[256];

        /* initial row: any of the 256 rows is allowed */
        for (int i = 0; i < 256; ++i)
            dp_curr[i] = 1;

        /* perform n transitions (n steps) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 256; ++i)
                dp_next[i] = 0;

            for (int r = 0; r < 256; ++r) {
                seq_t cur = dp_curr[r];
                if (cur == 0) continue;
                uint16_t d = deg[e][r];
                for (uint16_t k = 0; k < d; ++k) {
                    int r2 = neigh[e][r][k];
                    dp_next[r2] += cur;
                }
            }

            /* copy next state to current for the next step */
            for (int i = 0; i < 256; ++i)
                dp_curr[i] = dp_next[i];
        }

        /* sum over all ending rows */
        for (int i = 0; i < 256; ++i)
            total += dp_curr[i];
    }

    return total;
}
