/*
 * A236944  Number of (n+1)X(3+1) 0..3 arrays with the maximum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of (n+1)×4 arrays over {0,1,2,3} such that every
   2×2 subblock has the same maximum value */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute auxiliary data only once */
    static bool init_done = false;
    static unsigned char adj[16][16] = {0};
    static seq_t weight[4][16] = {0};
    if (!init_done) {
        /* zero count for each mask (0 … 15) */
        static const unsigned char zeros[16] = {
            4,3,3,2,3,2,2,1,3,2,2,1,2,1,1,0
        };

        /* powers for M = 0..3 */
        const seq_t pow2[5] = {1,2,4,8,16};
        const seq_t pow3[5] = {1,3,9,27,81};

        for (int m = 0; m <= 3; ++m) {
            for (int mask = 0; mask < 16; ++mask) {
                if (m == 0)
                    weight[m][mask] = (mask == 15 ? (seq_t)1 : (seq_t)0);
                else if (m == 1)
                    weight[m][mask] = (seq_t)1;
                else if (m == 2)
                    weight[m][mask] = pow2[zeros[mask]];
                else /* m==3 */
                    weight[m][mask] = pow3[zeros[mask]];
            }
        }

        /* adjacency matrix: (mask1, mask2) allowed? */
        for (int m1 = 0; m1 < 16; ++m1) {
            for (int m2 = 0; m2 < 16; ++m2) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    bool bit =
                        ((m1 >> j) & 1) | ((m1 >> (j+1)) & 1) |
                        ((m2 >> j) & 1) | ((m2 >> (j+1)) & 1);
                    if (!bit) { ok = false; break; }
                }
                adj[m1][m2] = ok ? 1 : 0;
            }
        }
        init_done = true;
    }

    /* R = number of rows = n+1 */
    idx_t R = n + (idx_t)1;
    seq_t total = 0;

    /* Dynamic programming for each M = 0..3 */
    for (int m = 0; m <= 3; ++m) {
        seq_t cur[16], nxt[16];
        /* first row */
        for (int mask = 0; mask < 16; ++mask)
            cur[mask] = weight[m][mask];

        /* subsequent rows */
        for (idx_t r = 1; r < R; ++r) {
            for (int mask = 0; mask < 16; ++mask) nxt[mask] = (seq_t)0;
            for (int mask1 = 0; mask1 < 16; ++mask1) {
                seq_t v = cur[mask1];
                if (!v) continue;
                for (int mask2 = 0; mask2 < 16; ++mask2) {
                    if (!adj[mask1][mask2]) continue;
                    /* v * weight[m][mask2] may overflow 64‑bit; seq_t handles it */
                    nxt[mask2] += v * weight[m][mask2];
                }
            }
            for (int mask = 0; mask < 16; ++mask) cur[mask] = nxt[mask];
        }

        /* sum over all ending masks to get b_m(n) */
        seq_t bm = 0;
        for (int mask = 0; mask < 16; ++mask) bm += cur[mask];
        total += bm;
    }

    return total;
}
