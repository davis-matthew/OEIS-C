/*
 * A128102  Number of 2 X 2 tiles in all tilings of a 4 X n rectangle with 1 X
 *          1 and 2 X 2 square tiles.
 * https://oeis.org/A128102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A128102: number of 2×2 tiles in all tilings of a 4×n rectangle
   with 1×1 and 2×2 tiles.  The implementation uses dynamic programming over
   the 16 column‑state masks. */

#include <stddef.h>   /* for size_t */

/* Transition information for a single column. */
typedef struct {
    int  nextmask;   /* bits occupied in the next column by half‑tiles */
    seq_t ways;      /* how many ways lead to this transition */
    seq_t tiles;     /* total number of 2×2 tiles over all those ways */
} Trans;

/* Storage for all transitions: 16 masks, each with at most 32 transitions. */
static Trans trans[16][32];
static int  transCnt[16];   /* number of transitions for each mask */

/* Recursive generator: fill a column given a current mask.
   curMask  – bits already occupied in this column (from previous half‑tiles)
   nextMask – bits that become occupied in the next column by 2×2 tiles started here
   tiles    – number of 2×2 tiles used so far in this column
   waysRem[nm] – number of ways leading to nextMask nm
   tilesRem[nm] – sum of 2×2 tiles used over those ways. */
static void dfs(int curMask, int nextMask, int tiles,
                seq_t *waysRem, seq_t *tilesRem)
{
    int r;
    for (r = 0; r < 4; ++r)
        if (!(curMask & (1 << r))) break;

    /* column finished */
    if (r == 4) {
        waysRem[nextMask]  += (seq_t)1;
        tilesRem[nextMask] += (seq_t)tiles;
        return;
    }

    /* place a 1×1 tile */
    dfs(curMask | (1 << r), nextMask, tiles, waysRem, tilesRem);

    /* place a 2×2 tile if possible */
    if (r + 1 < 4 && !(curMask & (1 << (r + 1)))) {
        int newMask   = curMask | (1 << r) | (1 << (r + 1));
        int newNext = nextMask | (1 << r) | (1 << (r + 1));
        dfs(newMask, newNext, tiles + 1, waysRem, tilesRem);
    }
}

/* Precompute all transitions for the 16 masks. */
static void init_trans(void)
{
    for (int mask = 0; mask < 16; ++mask) {
        seq_t waysRem[16]  = {0};
        seq_t tilesRem[16] = {0};
        dfs(mask, 0, 0, waysRem, tilesRem);

        int k = 0;
        for (int nm = 0; nm < 16; ++nm) {
            if (waysRem[nm]) {
                trans[mask][k].nextmask = nm;
                trans[mask][k].ways     = waysRem[nm];
                trans[mask][k].tiles    = tilesRem[nm];
                ++k;
            }
        }
        transCnt[mask] = k;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_trans();
        initialized = 1;
    }

    seq_t dp[16]   = {0};
    seq_t sum[16]  = {0};
    dp[0] = (seq_t)1;   /* start with no pending tiles */

    seq_t newdp[16];
    seq_t newsum[16];

    for (idx_t i = 0; i < n; ++i) {
        for (int m = 0; m < 16; ++m) {
            newdp[m]  = (seq_t)0;
            newsum[m] = (seq_t)0;
        }

        for (int m = 0; m < 16; ++m) {
            seq_t curWays = dp[m];
            if (curWays == (seq_t)0) continue;
            seq_t curSum = sum[m];

            for (int t = 0; t < transCnt[m]; ++t) {
                int nm = trans[m][t].nextmask;
                seq_t w   = trans[m][t].ways;
                seq_t ts  = trans[m][t].tiles;

                newdp[nm]  += curWays * w;
                newsum[nm] += curSum * w + curWays * ts;
            }
        }

        for (int m = 0; m < 16; ++m) {
            dp[m]  = newdp[m];
            sum[m] = newsum[m];
        }
    }

    return sum[0];   /* only tilings that finish without pending tiles */
}
