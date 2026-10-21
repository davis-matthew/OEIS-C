/*
 * A231801  Number of (n+1)X(3+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to one.
 * https://oeis.org/A231801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
   Counts the number of (n+1)×4 binary arrays in which no cell has a
   strict majority of its horizontal, vertical, diagonal, and
   antidiagonal neighbours equal to one.  This is done by a
   transfer‑matrix DP that processes the grid row by row.
   The state is the last two rows (masked as 4‑bit integers).  A helper
   function verifies whether a row is legal given its neighbours. */

#include <stdbool.h>
#include <stdint.h>

/* Check whether the middle row (maskB) of three consecutive rows
   (maskA – top, maskB – middle, maskC – bottom) satisfies the
   majority condition.  The boolean flags indicate whether the top or
   bottom neighbours exist (border rows). */
static inline bool row_valid(uint8_t maskA, uint8_t maskB, uint8_t maskC,
                             bool has_above, bool has_below)
{
    for (int j = 0; j < 4; ++j) {
        int ones = 0, neigh = 0;

        /* same‑row neighbours (left & right) */
        if (j > 0) {
            ++neigh;
            if ((maskB >> (j - 1)) & 1) ++ones;
        }
        if (j < 3) {
            ++neigh;
            if ((maskB >> (j + 1)) & 1) ++ones;
        }

        /* above neighbours */
        if (has_above) {
            ++neigh;
            if ((maskA >> j) & 1) ++ones;
            if (j > 0) {
                ++neigh;
                if ((maskA >> (j - 1)) & 1) ++ones;
            }
            if (j < 3) {
                ++neigh;
                if ((maskA >> (j + 1)) & 1) ++ones;
            }
        }

        /* below neighbours */
        if (has_below) {
            ++neigh;
            if ((maskC >> j) & 1) ++ones;
            if (j > 0) {
                ++neigh;
                if ((maskC >> (j - 1)) & 1) ++ones;
            }
            if (j < 3) {
                ++neigh;
                if ((maskC >> (j + 1)) & 1) ++ones;
            }
        }

        /* strict majority check: ones > neigh/2  <=>  2*ones > neigh */
        if (ones * 2 > neigh) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    const idx_t rows = n + 1;       /* number of rows */

    /* Pre‑compute validity tables once */
    static bool first_valid[16][16];
    static bool internal_valid[16][16][16];
    static bool tables_built = false;

    if (!tables_built) {
        /* first row: no above neighbour, below exists */
        for (int r0 = 0; r0 < 16; ++r0)
            for (int r1 = 0; r1 < 16; ++r1)
                first_valid[r0][r1] = row_valid(0, (uint8_t)r0, (uint8_t)r1,
                                                false, true);

        /* internal rows: both neighbours exist */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                for (int nrow = 0; nrow < 16; ++nrow)
                    internal_valid[p][c][nrow] =
                        row_valid((uint8_t)p, (uint8_t)c, (uint8_t)nrow,
                                  true, true);

        tables_built = true;
    }

    /* DP: state (prev_row<<4 | cur_row), 256 possible states */
    static seq_t dp[256];
    static seq_t next[256];

    /* initialise DP with the first two rows */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            if (first_valid[r0][r1])
                dp[(r0 << 4) | r1] += (seq_t)1;

    /* process rows 3 .. rows (0‑based indexing) */
    for (idx_t row = 2; row < rows; ++row) {
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int state = 0; state < 256; ++state) {
            seq_t ways = dp[state];
            if (ways == 0) continue;
            uint8_t prev = state >> 4;
            uint8_t cur  = state & 0xF;
            for (int nxt = 0; nxt < 16; ++nxt)
                if (internal_valid[prev][cur][nxt])
                    next[(cur << 4) | nxt] += ways;
        }
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* final (bottom) row must be valid without a below neighbour */
    seq_t result = 0;
    for (int state = 0; state < 256; ++state) {
        seq_t ways = dp[state];
        if (ways == 0) continue;
        uint8_t prev = state >> 4;
        uint8_t cur  = state & 0xF;
        if (row_valid(prev, cur, 0, true, false))
            result += ways;
    }
    return result;
}
