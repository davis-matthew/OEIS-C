/*
 * A186056  Number of (n+2)X4 binary arrays with each 3X3 subblock having a
 *          positive determinant.
 * https://oeis.org/A186056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (n+2)×4 binary arrays with all 3×3 subblocks
 *            positive
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   det3x3_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186056
   a(n) = Number of (n+2)×4 binary arrays with each 3×3 subblock having a positive determinant.
   Computation is done by dynamic programming on the last two rows. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/det3x3_int.h"

/* ------------------------------------------------------------------ */
/* Helper: 3×3 determinant of rows r1,r2,r3 on columns col..col+2   */

/* ------------------------------------------------------------------ */
/* Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Transitions from state (prevprev,prev) to next state (prev,next).   */
    static unsigned char nextRows[256][16];
    static unsigned char nextCount[256];
    static bool   built = false;

    if (!built)
    {
        /* Pre‑compute which triples of rows satisfy the determinant condition. */
        unsigned char allowed[16][16][16];
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
                for (int r3 = 0; r3 < 16; ++r3)
                {
                    int d0 = det3x3_int(r1, r2, r3, 0);
                    int d1 = det3x3_int(r1, r2, r3, 1);
                    allowed[r1][r2][r3] = (unsigned char)((d0 > 0) && (d1 > 0));
                }

        /* Build adjacency lists. */
        for (int s = 0; s < 256; ++s)
        {
            nextCount[s] = 0;
            int prevprev = s >> 4;
            int prev     = s & 0xF;
            for (int next = 0; next < 16; ++next)
                if (allowed[prevprev][prev][next])
                    nextRows[s][nextCount[s]++] = (unsigned char)next;
        }
        built = true;
    }

    /* Dynamic programming over rows.  */
    seq_t dp[256];
    seq_t ndp[256];
    for (int i = 0; i < 256; ++i) dp[i] = 1;   /* length = 2 rows: every pair is possible */

    idx_t steps = n;                /* we need to add n more rows to reach (n+2) rows */
    for (idx_t k = 0; k < steps; ++k)
    {
        for (int i = 0; i < 256; ++i) ndp[i] = 0;
        for (int s = 0; s < 256; ++s)
        {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            unsigned char cnt = nextCount[s];
            int base = (s & 0xF) << 4;   /* new state will have prev = old prev */
            for (int j = 0; j < cnt; ++j)
            {
                int ns = base | nextRows[s][j];
                ndp[ns] += cur;
            }
        }
        for (int i = 0; i < 256; ++i) dp[i] = ndp[i];
    }

    /* Sum over all ending states. */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) result += dp[i];
    return result;
}
