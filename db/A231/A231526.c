/*
 * A231526  Number of 4Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 4×n binary arrays with * no element less than a
 *            strict majority of its horizontal, * diagonal and antidiagonal
 *            neighbors. * * The value is computed by dynamic programming over
 *            columns. * For a 4×n grid the validator checks that for every
 *            0–cell * the number of 1-neighbours does not ex...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool tables_computed = false;
static bool left_valid[16][16];
static bool middle_valid[16][16][16];

/* Validate one column 'col' when its left neighbour is 'l' and
 * right neighbour is 'r'.  The arguments has_left/right indicate
 * whether the left/right neighbour actually exists. */
static bool column_valid(uint8_t col, uint8_t l, uint8_t r,
                         bool has_left, bool has_right)
{
    for (int row = 0; row < 4; ++row) {
        uint8_t cell = (col >> row) & 1;
        int ones   = 0;
        int neighs = 0;

        /* left neighbour same row */
        if (has_left) {
            if ((l >> row) & 1) ++ones;
            ++neighs;
        }
        /* right neighbour same row */
        if (has_right) {
            if ((r >> row) & 1) ++ones;
            ++neighs;
        }
        /* left diagonal up */
        if (has_left && row > 0) {
            if ((l >> (row - 1)) & 1) ++ones;
            ++neighs;
        }
        /* left diagonal down */
        if (has_left && row < 3) {
            if ((l >> (row + 1)) & 1) ++ones;
            ++neighs;
        }
        /* right diagonal up */
        if (has_right && row > 0) {
            if ((r >> (row - 1)) & 1) ++ones;
            ++neighs;
        }
        /* right diagonal down */
        if (has_right && row < 3) {
            if ((r >> (row + 1)) & 1) ++ones;
            ++neighs;
        }

        if (cell == 0) {
            /* must not have a strict majority of 1‑neighbours */
            if (ones * 2 > neighs)
                return false;
        }
    }
    return true;
}

/* Pre‑compute the neighbour tables once. */
static void init_tables(void)
{
    if (tables_computed) return;
    for (int c = 0; c < 16; ++c) {
        for (int l = 0; l < 16; ++l) {
            /* left neighbour only (no right) */
            left_valid[c][l] = column_valid((uint8_t)c, (uint8_t)l, 0,
                                            true, false);
        }
    }
    for (int l = 0; l < 16; ++l) {
        for (int m = 0; m < 16; ++m) {
            for (int r = 0; r < 16; ++r) {
                /* both neighbours present */
                middle_valid[l][m][r] =
                    column_valid((uint8_t)m, (uint8_t)l, (uint8_t)r,
                                 true, true);
            }
        }
    }
    tables_computed = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_tables();

    if (n == 1) return (seq_t)16;      /* 2^4 */

    /* dp[prev][curr] : number of ways up to current column 'curr',
     *   with previous column 'prev' already validated.
     *   The array is stored as a flat 256 entry array. */
    static seq_t dp[256];
    static seq_t next[256];

    /* Initialize with columns 0 and 1.
     * Column 0 must be valid with its right neighbour only. */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    for (int c0 = 0; c0 < 16; ++c0) {
        for (int c1 = 0; c1 < 16; ++c1) {
            if (column_valid((uint8_t)c0, (uint8_t)c1, 0,
                             true, false)) {
                dp[(c0 << 4) | c1] = 1;
            }
        }
    }

    /* Process columns 2 .. n-1, if any. */
    for (idx_t col = 2; col < n; ++col) {
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int prevprev = 0; prevprev < 16; ++prevprev) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t ways = dp[(prevprev << 4) | prev];
                if (!ways) continue;
                for (int cur = 0; cur < 16; ++cur) {
                    if (middle_valid[prevprev][prev][cur]) {
                        next[(prev << 4) | cur] += ways;
                    }
                }
            }
        }
        /* move next into dp */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* Final column (col n-1) has no right neighbour.
     * Validate it with its left neighbour only, and sum. */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (dp[(prev << 4) | curr] &&
                left_valid[curr][prev]) {
                result += dp[(prev << 4) | curr];
            }
        }
    }
    return result;
}
