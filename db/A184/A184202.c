/*
 * A184202  Number of (n+1)X5 binary arrays with no 2X2 subblock containing
 *          fewer than two 1s.
 * https://oeis.org/A184202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computed by dynamic programming over 5‑column binary rows
 *            (states are 5‑bit masks). a(n) counts (n+1)×5 arrays with no
 *            sub‑block having fewer than two 1’s. Same result as the given
 *            19‑term linear recurrence
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The function is required to have external linkage */
seq_t a(idx_t n)
{
    /* Precomputation of allowed transitions between row masks,
       done only once.  The state space is all 5‑bit masks (0…31). */
    static bool init = false;
    static int next_mask[32][32];
    static int next_cnt[32];

    if (!init) {
        for (int m = 0; m < 32; ++m) {
            int cnt = 0;
            for (int m2 = 0; m2 < 32; ++m2) {
                bool ok = true;
                /* check each 2×2 block formed by m (above) and m2 (below) */
                for (int j = 0; j < 4; ++j) {
                    int a1 = (m >> j) & 1;
                    int b1 = (m >> (j + 1)) & 1;
                    int a2 = (m2 >> j) & 1;
                    int b2 = (m2 >> (j + 1)) & 1;
                    int s = a1 + b1 + a2 + b2;
                    if (s < 2) { ok = false; break; }
                }
                if (ok) {
                    next_mask[m][cnt++] = m2;
                }
            }
            next_cnt[m] = cnt;
        }
        init = true;
    }

    /* Number of rows in the array is n+1.  The first row is arbitrary. */
    idx_t rows = n + 1;

    seq_t cur[32], next[32];
    /* Initialise counts for the first row: one array for each mask */
    for (int i = 0; i < 32; ++i) cur[i] = 1;

    /* Build subsequent rows */
    for (idx_t r = 2; r <= rows; ++r) {
        for (int i = 0; i < 32; ++i) next[i] = 0;
        for (int m = 0; m < 32; ++m) {
            seq_t val = cur[m];
            if (val == 0) continue;
            int cnt = next_cnt[m];
            for (int k = 0; k < cnt; ++k) {
                int m2 = next_mask[m][k];
                next[m2] += val;
            }
        }
        for (int i = 0; i < 32; ++i) cur[i] = next[i];
    }

    /* Sum over all possibilities for the last row */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i) total += cur[i];
    return total;
}
