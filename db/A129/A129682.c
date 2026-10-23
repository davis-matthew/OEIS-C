/*
 * A129682  Number of ways tiling a 2 X n rectangle with 2 X 1 (domino) and 3
 *          X 1 (tromino) tiles.
 * https://oeis.org/A129682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ways to tile a 2×n rectangle with 2×1 dominoes
 *            (horizontal or vertical) and 3×1 trominoes (horizontal only).
 *            The algorithm uses a DP over columns with a 4‑bit mask that
 *            encodes the occupancy of the current column and the next column.
 *            For each column all possibilities of covering...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void dfs_fill(int cur_low, int new_low, int new_high, int rem,
                     seq_t mult, seq_t *newdp)
{
    /* find first free cell in the current column */
    int r;
    for (r = 0; r < 2; ++r)
        if (!(cur_low & (1 << r)))
            break;

    if (r == 2) {
        /* the current column is completely covered – record the next state */
        int next_mask = (new_high << 2) | new_low;
        newdp[next_mask] += mult;
        return;
    }

    /* 1. vertical domino (covers both rows of the current column) */
    if (r == 0 && !(cur_low & 2)) {
        dfs_fill(cur_low | 3, new_low, new_high, rem, mult, newdp);
    }

    /* 2. horizontal domino in row r (uses next column) */
    if (rem >= 2 && !(new_low & (1 << r))) {
        dfs_fill(cur_low | (1 << r),
                 new_low | (1 << r),
                 new_high,
                 rem,
                 mult,
                 newdp);
    }

    /* 3. tromino in row r (uses next and the column after next) */
    if (rem >= 3 && !(new_low & (1 << r))) {
        dfs_fill(cur_low | (1 << r),
                 new_low | (1 << r),
                 new_high | (1 << r),
                 rem,
                 mult,
                 newdp);
    }
}

seq_t a(idx_t n)
{
    /* dp[mask] holds the number of ways for the first processed columns;
       mask is a 4‑bit integer: low 2 bits = occupancy of current column,
       high 2 bits = occupancy of next column. */
    seq_t dp[16] = {0};
    seq_t newdp[16];
    dp[0] = 1;                  /* empty board – no pending cells */

    for (idx_t i = 0; i < n; ++i) {
        /* clear newdp */
        for (int j = 0; j < 16; ++j)
            newdp[j] = 0;

        /* remaining columns including current */
        int rem = (int)(n - i);

        for (int m = 0; m < 16; ++m) {
            seq_t ways = dp[m];
            if (ways == 0)
                continue;

            int cur_low  =  m & 3;          /* occupancy of current column */
            int cur_high = (m >> 2) & 3;    /* occupancy of next column */

            /* the occupancy that already exists in the next column will be part of new_low */
            int init_new_low  = cur_high;
            int init_new_high = 0;

            dfs_fill(cur_low,
                     init_new_low,
                     init_new_high,
                     rem,
                     ways,
                     newdp);
        }

        /* copy newdp into dp for the next iteration */
        for (int j = 0; j < 16; ++j)
            dp[j] = newdp[j];
    }

    /* after processing all n columns, we want no pending cells */
    return dp[0];
}
