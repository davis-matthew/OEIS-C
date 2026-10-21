/*
 * A237236  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3} with the
 *            expression max + lower median – upper median – min of every 2×2
 *            subblock equal throughout the array
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   block_expr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/block_expr.h"

/* extract the 2‑bit digit at column j (0..2) of a 3‑digit base‑4 row */
static inline int val_at(unsigned int row, int j)
{
    return (row >> (2 * j)) & 3;
}

/* adjacency matrices: adj[C][i][j] = 1 if rows i→j create the same
   block expression value C for the two overlapping 2×2 blocks */
static bool init_done = false;
static uint8_t adj[4][64][64] = {0};

static void build_adjacency(void)
{
    for (int c = 0; c < 4; ++c)
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                adj[c][i][j] = 0;

    for (int r1 = 0; r1 < 64; ++r1) {
        for (int r2 = 0; r2 < 64; ++r2) {
            int e0 = block_expr(val_at(r1, 0), val_at(r1, 1),
                                val_at(r2, 0), val_at(r2, 1));
            int e1 = block_expr(val_at(r1, 1), val_at(r1, 2),
                                val_at(r2, 1), val_at(r2, 2));
            if (e0 != e1) continue;
            int C = e0;
            if (C < 0 || C > 3) continue;           /* safety */
            adj[C][r1][r2] = 1;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) build_adjacency();

    seq_t result = 0;

    for (int C = 0; C < 4; ++C) {
        seq_t v[64];
        seq_t nv[64];

        /* any first row is allowed */
        for (int i = 0; i < 64; ++i) v[i] = 1;

        /* perform n transitions: from first row to the (n+1)-th row */
        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < 64; ++j) nv[j] = 0;

            for (int i = 0; i < 64; ++i) {
                if (v[i] == 0) continue;
                for (int j = 0; j < 64; ++j) {
                    if (adj[C][i][j]) nv[j] += v[i];
                }
            }

            /* copy nv back into v for next iteration */
            for (int i = 0; i < 64; ++i) v[i] = nv[i];
        }

        /* sum all sequences ending in any last row */
        for (int i = 0; i < 64; ++i) result += v[i];
    }

    return result;
}
