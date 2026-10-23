/*
 * A184205  Number of (n+1) X 8 binary arrays with no 2 X 2 subblock
 *          containing fewer than two 1's.
 * https://oeis.org/A184205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of (n+1) × 8 binary arrays with no 2×2 subblock containing fewer than two 1's.
   Computed by dynamic programming over 8-bit row patterns. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Precompute adjacency between 8‑bit rows. */
    static bool ready = false;
    static unsigned char neighbors[256][256];
    static unsigned short deg[256];   /* may be up to 256 */

    if (!ready) {
        for (int p = 0; p < 256; ++p) {
            unsigned short d = 0;
            for (int q = 0; q < 256; ++q) {
                bool ok = true;
                for (int j = 0; j < 7; ++j) {
                    int s = ((p >> j) & 1) + ((p >> (j + 1)) & 1)
                           + ((q >> j) & 1) + ((q >> (j + 1)) & 1);
                    if (s < 2) { ok = false; break; }
                }
                if (ok) {
                    neighbors[p][d++] = (unsigned char)q;
                }
            }
            deg[p] = d;
        }
        ready = true;
    }

    /* DP arrays: number of ways to end with each 8‑bit pattern. */
    static seq_t dp[256];
    static seq_t nxt[256];

    /* Base case: first row can be any 8‑bit pattern. */
    for (int i = 0; i < 256; ++i) dp[i] = seq_one();

    /* Each step adds a new row. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;
        for (int p = 0; p < 256; ++p) {
            seq_t val = dp[p];
            if (val == 0) continue;
            unsigned short d = deg[p];
            for (unsigned short k = 0; k < d; ++k) {
                unsigned char q = neighbors[p][k];
                nxt[q] += val;
            }
        }
        /* Move to next step */
        for (int i = 0; i < 256; ++i) dp[i] = nxt[i];
    }

    /* Sum over all possible last rows. */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) result += dp[i];
    return result;
}
