/*
 * A063000  Number of paths of length n+2 originating at a corner of a 4 X 4
 *          Boggle board.
 * https://oeis.org/A063000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   ctz, popcnt16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A063000 : Number of simple paths of length n+2 vertices
    starting at a corner of a 4×4 Boggle board (edges are orthogonal or diagonal).
    The sequence is for n ≥ 0. For n > 14 there are no paths (return 0). */

#include <stdbool.h>
#include <string.h>
#include "../utils/ctz.h"
#include "../utils/popcnt16.h"

#define N     16                      /* number of vertices of a 4×4 board          */
#define FULL  (1u << N)              /* number of vertex subsets (0 … 65535)       */

static seq_t totals[15];              /* totals[n] = a(n) for 0 ≤ n ≤ 14          */
static bool initialized = false;

/* Compute all values once and fill 'totals'                       */
static void init_sequence(void)
{
    if (initialized) return;

    /* adjacency masks for each vertex                                 */
    unsigned neighbor_mask[N];
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            int v = r * 4 + c;
            unsigned m = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4) {
                        m |= 1u << (nr * 4 + nc);
                    }
                }
            }
            neighbor_mask[v] = m;
        }
    }

    /* dp[mask][v] – number of simple paths that visit exactly
       the vertices in ‘mask’ and end at vertex ‘v’.               */
    static seq_t dp[FULL][N];      /* static to avoid stack overflow */
    memset(dp, 0, sizeof(dp));

    /* start at corner vertex 0 (top‑left)                            */
    dp[1u << 0][0] = 1;

    /* enumerate all masks in increasing order                         */
    for (unsigned mask = 0; mask < FULL; ++mask) {
        for (int v = 0; v < N; ++v) {
            seq_t cur = dp[mask][v];
            if (!cur) continue;
            unsigned avail = neighbor_mask[v] & ~mask;
            while (avail) {
                unsigned bit  = avail & -avail;   /* lowest set bit */
                int next = ctz(bit);
                dp[mask | (1u << next)][next] += cur;
                avail -= bit;
            }
        }
    }

    /* accumulate totals for each visited‑vertex count (m)             */
    memset(totals, 0, sizeof(totals));                 /* just in case  */
    for (unsigned mask = 0; mask < FULL; ++mask) {
        int m = popcnt16(mask);
        if (m < 2) continue;          /* need at least two vertices (path length ≥ 2) */
        seq_t sum = 0;
        for (int v = 0; v < N; ++v) sum += dp[mask][v];
        totals[m - 2] += sum;         /* accumulate over all masks of the same size */
    }

    initialized = true;
}

/* Public function required by the build            */
seq_t a(idx_t n)
{
    init_sequence();
    if (n > 14) return 0;
    return totals[n];
}
