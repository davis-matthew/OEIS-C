/*
 * A206996  Number of nX4 0..1 arrays avoiding 0 1 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over the 12 valid 4‑bit rows,
 *            counting n×4 binary arrays that avoid the horizontal pattern
 *            0‑1‑0 and the vertical pattern 0‑1‑1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Valid 4‑bit rows (no horizontal 0‑1‑0). */
    static const int row_bits[12] = {
        0,  1,  3,  6,  7,
        8,  9, 11, 12, 13, 14, 15
    };

    /* allowed[x][y][z] == true iff a vertical stack of
       row x (top), y (middle), z (bottom) contains no 0‑1‑1
       in any column. */
    static bool allowed[12][12][12] = {0};

    /* DP tables: dp_cur[x][y] = number of arrays ending with rows
       x (penultimate) and y (last). */
    static seq_t dp_cur[12][12];
    static seq_t dp_next[12][12];

    /* Cache of totals for lengths up to the largest computed. */
    enum { MAXN = 10005 };           /* enough for n≈10000                        */
    static seq_t totals[MAXN];
    static bool init = false;
    static idx_t current_len = 0;    /* last length for which dp_cur is valid */

    /* One‑time initialization. */
    if (!init)
    {
        /* Build the allowed matrix. */
        for (int x = 0; x < 12; ++x)
            for (int y = 0; y < 12; ++y)
                for (int z = 0; z < 12; ++z) {
                    bool ok = true;
                    for (int j = 0; j < 4; ++j) {
                        int xb = (row_bits[x] >> j) & 1;
                        int yb = (row_bits[y] >> j) & 1;
                        int zb = (row_bits[z] >> j) & 1;
                        if (xb == 0 && yb == 1 && zb == 1) {
                            ok = false;
                            break;
                        }
                    }
                    allowed[x][y][z] = ok;
                }

        /* Initial DP for length 2: every pair of valid rows is allowed. */
        for (int x = 0; x < 12; ++x)
            for (int y = 0; y < 12; ++y)
                dp_cur[x][y] = (seq_t)1;

        totals[1] = (seq_t)12;   /* number of valid rows */
        totals[2] = (seq_t)144;  /* 12 × 12 */
        current_len = 2;
        init = true;
    }

    if (n <= 2)
        return totals[n];

    /* Extend cache up to n if necessary. */
    while (current_len < n) {
        /* Compute dp_next for length current_len + 1. */
        for (int y = 0; y < 12; ++y)
            for (int z = 0; z < 12; ++z) {
                seq_t sum = 0;
                for (int x = 0; x < 12; ++x)
                    if (allowed[x][y][z])
                        sum += dp_cur[x][y];
                dp_next[y][z] = sum;
            }

        /* Sum all (y,z) to obtain the total for the new length. */
        seq_t total = 0;
        for (int y = 0; y < 12; ++y)
            for (int z = 0; z < 12; ++z)
                total += dp_next[y][z];

        ++current_len;
        totals[current_len] = total;

        /* Prepare dp_cur for the next iteration. */
        for (int y = 0; y < 12; ++y)
            for (int z = 0; z < 12; ++z)
                dp_cur[y][z] = dp_next[y][z];
    }

    return totals[n];
}
