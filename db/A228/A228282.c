/*
 * A228282  Number of nX7 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of n×7 binary arrays with top‑left cell 1
 *            and * no two 1‑cells adjacent horizontally, vertically or
 *            northwest‑southeast * diagonally. Computed by dynamic
 *            programming over 7‑bit row masks
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MASKS 128   /* 2^7 possible row masks */

seq_t a(idx_t n)
{
    /* Pre‑compute which masks are internally valid (no adjacent 1‑bits). */
    bool valid[MASKS];
    for (int m = 0; m < MASKS; ++m)
        valid[m] = ((m & (m << 1)) == 0);

    /* DP tables: dpPrev holds counts for the last processed row,
     * dpCurr holds counts for the current row. */
    seq_t dpPrev[MASKS] = {0};
    seq_t dpCurr[MASKS] = {0};

    /* ----- initialise for the first row (row index 1) ----- */
    /* The top‑left cell (bit 0) must be 1. */
    for (int m = 0; m < MASKS; ++m) {
        if (valid[m] && (m & 1))
            dpPrev[m] = 1;
    }

    /* ----- process rows 2..n ----- */
    for (idx_t r = 2; r <= n; ++r) {
        for (int m = 0; m < MASKS; ++m) {
            if (!valid[m]) {
                dpCurr[m] = 0;
                continue;
            }
            seq_t sum = 0;
            for (int p = 0; p < MASKS; ++p) {
                if (!valid[p]) continue;
                /* vertical adjacency: same column must not both be 1 */
                if (m & p) continue;
                /* nw‑se diagonal adjacency: bit i in m cannot align
                 * with bit i‑1 in p (i.e., (m & (p << 1)) != 0). */
                if (m & (p << 1)) continue;
                sum += dpPrev[p];
            }
            dpCurr[m] = sum;
        }
        /* swap tables for next iteration */
        for (int m = 0; m < MASKS; ++m)
            dpPrev[m] = dpCurr[m];
    }

    /* ----- sum all configurations of the last row ----- */
    seq_t result = 0;
    for (int m = 0; m < MASKS; ++m)
        if (valid[m])
            result += dpPrev[m];

    return result;
}
