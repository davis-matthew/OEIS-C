/*
 * A025603  Number of n-move queen paths on 8x8 board from given corner to any
 *          square.
 * https://oeis.org/A025603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑move queen paths on an 8×8 board starting
 *            from one corner and ending at any square
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute adjacent squares for each of the 64 board positions. */
    static bool init = false;
    static uint8_t neigh[64][28];
    static uint8_t neigh_len[64];
    if (!init) {
        const int dirs[8][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1},
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        };
        for (int u = 0; u < 64; ++u) {
            int r = u / 8;
            int c = u % 8;
            int cnt = 0;
            for (int d = 0; d < 8; ++d) {
                int dr = dirs[d][0];
                int dc = dirs[d][1];
                int rr = r + dr;
                int cc = c + dc;
                while (rr >= 0 && rr < 8 && cc >= 0 && cc < 8) {
                    neigh[u][cnt++] = (uint8_t)(rr * 8 + cc);
                    rr += dr;
                    cc += dc;
                }
            }
            neigh_len[u] = (uint8_t)cnt;
        }
        init = true;
    }

    /* A small dynamic‑programming table for the current and next step. */
    static seq_t dp[64];
    static seq_t next[64];

    /* Start from the corner square at step 0. */
    for (int i = 0; i < 64; ++i) dp[i] = 0;
    dp[0] = 1;                     /* corner (0,0) */
    if (n == 0) return 1;          /* a(0) = 1 */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) next[i] = 0;

        for (int u = 0; u < 64; ++u) {
            seq_t val = dp[u];
            if (val == 0) continue;
            const uint8_t *adj = neigh[u];
            int len = neigh_len[u];
            for (int k = 0; k < len; ++k) {
                next[adj[k]] += val;
            }
        }

        /* Sum over all ending squares after this step. */
        seq_t tot = 0;
        for (int i = 0; i < 64; ++i) tot += next[i];
        if (step == n) return tot;

        /* Prepare for the next iteration. */
        for (int i = 0; i < 64; ++i) dp[i] = next[i];
    }

    return 0;  /* unreachable */
}
