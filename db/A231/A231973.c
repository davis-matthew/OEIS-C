/*
 * A231973  Number of (n+1)X(4+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal and vertical neighbors equal to one.
 * https://oeis.org/A231973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes OEIS sequence A231973. It uses a dynamic
 *            programming over rows of a (n+1)×5 binary matrix. For each cell,
 *            it forbids a strict majority of its horizontal and vertical
 *            neighbors to be 1. The DP keeps track of the last two rows and
 *            ensures all constraints are satisfied when en...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation works for any seq_t/idx_t types provided
   by the build system. It does not use any disallowed features. */

#include <stdbool.h>

#define WIDTH 5
#define MASKS (1 << WIDTH)

/* Precompute bit values of every mask for speed. */
static unsigned char mask_bits[MASKS][WIDTH];

/* Helper to set up mask_bits, called once per function call. */
static void init_mask_bits(void)
{
    static bool initialized = false;
    if (initialized) return;
    for (int m = 0; m < MASKS; ++m) {
        for (int c = 0; c < WIDTH; ++c) {
            mask_bits[m][c] = (m >> c) & 1u;
        }
    }
    initialized = true;
}

/* Determine if top row mask0 and second row mask1 satisfy
   the constraint for the top row (no up neighbors, has down). */
static bool valid_top_row(int mask0, int mask1)
{
    for (int c = 0; c < WIDTH; ++c) {
        int neighbors = 0;
        int count = 0;
        if (c > 0) { neighbors++; count += mask_bits[mask0][c - 1]; }
        if (c < WIDTH - 1) { neighbors++; count += mask_bits[mask0][c + 1]; }
        /* down neighbor exists */
        neighbors++;
        count += mask_bits[mask1][c];
        if (count * 2 > neighbors) return false;
    }
    return true;
}

/* Bottom row constraint (has up neighbors, no down). */
static bool valid_bottom_row(int maskPrev, int maskCur)
{
    for (int c = 0; c < WIDTH; ++c) {
        int neighbors = 0;
        int count = 0;
        if (c > 0) { neighbors++; count += mask_bits[maskCur][c - 1]; }
        if (c < WIDTH - 1) { neighbors++; count += mask_bits[maskCur][c + 1]; }
        /* up neighbor exists */
        neighbors++;
        count += mask_bits[maskPrev][c];
        if (count * 2 > neighbors) return false;
    }
    return true;
}

/* Middle rows: all three rows known. */
static bool valid_mid_rows(int maskPrev, int maskCur, int maskNext)
{
    for (int c = 0; c < WIDTH; ++c) {
        int neighbors = 0;
        int count = 0;
        if (c > 0) { neighbors++; count += mask_bits[maskCur][c - 1]; }
        if (c < WIDTH - 1) { neighbors++; count += mask_bits[maskCur][c + 1]; }
        /* up and down neighbors */
        neighbors += 2;
        count += mask_bits[maskPrev][c];
        count += mask_bits[maskNext][c];
        if (count * 2 > neighbors) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is the offset-relative index, a(1) is first term. */
    if (n == 0) return 0;   /* undefined but safe */
    init_mask_bits();

    const int rows = (int)(n + 1);  /* R = n + 1 */
    /* DP arrays: dp[prev][cur] holds number of ways for processed rows
       where the last two rows are prev and cur. */
    static seq_t dp[MASKS][MASKS];
    static seq_t dp_next[MASKS][MASKS];
    /* Zero-initialize dp */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp[i][j] = 0;

    /* Initial pair of rows (top and second). */
    for (int m0 = 0; m0 < MASKS; ++m0) {
        for (int m1 = 0; m1 < MASKS; ++m1) {
            if (valid_top_row(m0, m1))
                dp[m0][m1] = 1;
        }
    }

    /* If we only have two rows, handle bottom constraint directly. */
    if (rows == 2) {
        seq_t ans = 0;
        for (int m0 = 0; m0 < MASKS; ++m0) {
            for (int m1 = 0; m1 < MASKS; ++m1) {
                if (valid_bottom_row(m0, m1))
                    ans += dp[m0][m1];
            }
        }
        return ans;
    }

    /* Process remaining rows [2 .. rows-1] */
    for (int r = 2; r < rows; ++r) {
        /* Zero dp_next */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_next[i][j] = 0;
        /* Transition */
        for (int prev = 0; prev < MASKS; ++prev) {
            for (int cur = 0; cur < MASKS; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int next = 0; next < MASKS; ++next) {
                    if (!valid_mid_rows(prev, cur, next))
                        continue;
                    dp_next[cur][next] += ways;
                }
            }
        }
        /* Swap dp and dp_next */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* After all rows processed, enforce bottom row constraint. */
    seq_t ans = 0;
    for (int prev = 0; prev < MASKS; ++prev) {
        for (int cur = 0; cur < MASKS; ++cur) {
            if (dp[prev][cur] == 0) continue;
            if (valid_bottom_row(prev, cur))
                ans += dp[prev][cur];
        }
    }
    return ans;
}
