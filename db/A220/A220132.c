/*
 * A220132  Number of tilings of an n X 7 rectangle using integer-sided
 *          rectangular tiles of area n.
 * https://oeis.org/A220132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of tilings of an n × 7 rectangle using exactly one
 *            tile of area n for each of the 7 tiles (computed by exhaustive
 *            search)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Recursive helper: returns the number of tilings for the current board. */
static seq_t dfs_helper(int rows, uint8_t *mask,
                        int sh_cnt, const int *sh_h, const int *sh_w)
{
    /* Find the first empty cell in row‑major order. */
    int r = 0;
    for (; r < rows; ++r) {
        if (mask[r] != 0x7F) {          /* 0x7F = 01111111b : row full */
            break;
        }
    }
    if (r == rows)                      /* all rows occupied */
        return 1;

    /* Find the first free column in this row. */
    uint8_t free_bits = ~mask[r] & 0x7F;
    int c = 0;
    while (c < 7 && !(free_bits & (1u << c)))
        ++c;

    seq_t count = 0;
    /* Try all tile shapes at position (r, c). */
    for (int i = 0; i < sh_cnt; ++i) {
        int h = sh_h[i];
        int w = sh_w[i];
        if (r + h > rows || c + w > 7)
            continue;

        uint8_t mask_w = ((1u << w) - 1u) << c;   /* columns to occupy */
        bool ok = true;
        for (int rr = r; rr < r + h; ++rr) {
            if (mask[rr] & mask_w) {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        /* Place the tile. */
        for (int rr = r; rr < r + h; ++rr)
            mask[rr] |= mask_w;

        /* Recurse. */
        count += dfs_helper(rows, mask, sh_cnt, sh_h, sh_w);

        /* Remove the tile (backtrack). */
        for (int rr = r; rr < r + h; ++rr)
            mask[rr] ^= mask_w;
    }
    return count;
}

/* Compute a(n) for 0 ≤ n < 420 using exhaustive search. */
static seq_t compute_small(idx_t n)
{
    if (n == 0)
        return 1;

    /* Row occupancy masks (7 bits each). */
    uint8_t row_mask[420] = {0};

    /* Collect all unique tile shapes of area n with width ≤ 7. */
    int sh_h[16], sh_w[16];
    int sh_cnt = 0;

    for (int d = 1; d <= 7; ++d) {
        if (n % d != 0)
            continue;
        int h = (int)n / d;
        int w = d;                  /* width ≤ 7 by loop condition */

        /* Add shape (h, w). */
        bool dup = false;
        for (int j = 0; j < sh_cnt; ++j) {
            if (sh_h[j] == h && sh_w[j] == w) { dup = true; break; }
        }
        if (!dup) { sh_h[sh_cnt] = h; sh_w[sh_cnt] = w; ++sh_cnt; }

        /* Rotated shape if distinct and width ≤ 7. */
        if (h != w && h <= 7) {
            int rh = w;   /* rotated h */
            int rw = h;   /* rotated w */
            dup = false;
            for (int j = 0; j < sh_cnt; ++j) {
                if (sh_h[j] == rh && sh_w[j] == rw) { dup = true; break; }
            }
            if (!dup) { sh_h[sh_cnt] = rh; sh_w[sh_cnt] = rw; ++sh_cnt; }
        }
    }

    /* Recursive tiling search. */
    return dfs_helper((int)n, row_mask, sh_cnt, sh_h, sh_w);
}

/* Memoised periodic table for values 0 … 419. */
static seq_t period[420];
static bool period_ready = false;

/* Main entry function. */
seq_t a(idx_t n)
{
    if (!period_ready) {
        for (int i = 0; i < 420; ++i)
            period[i] = compute_small((idx_t)i);
        period_ready = true;
    }
    return period[(int)(n % 420)];
}
