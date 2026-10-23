/*
 * A183445  Number of n X 4 binary arrays with every 1 having exactly two
 *          king-move neighbors equal to 1.
 * https://oeis.org/A183445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over rows (transfer matrix).
   Counts n×4 binary arrays where each 1 has exactly two king‑move
   neighbors.  No closed formula is known; the program
   precomputes a validity table for every triple of consecutive rows
   and then iterates over the rows in O(n) time. */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a row is valid when the rows immediately above and below
   are given by the bitmasks `up` and `down`.  Each 1 in `row` must
   have exactly two king‑move neighbors equal to 1 (including the
   diagonals). */
static inline bool is_valid_row(int row, int up, int down)
{
    for (int j = 0; j < 4; ++j) {
        if (((row >> j) & 1) == 0) continue;
        int neigh = 0;
        /* horizontal neighbors */
        if (j > 0 && ((row >> (j - 1)) & 1)) ++neigh;
        if (j < 3 && ((row >> (j + 1)) & 1)) ++neigh;
        /* up row */
        if (j > 0 && ((up >> (j - 1)) & 1)) ++neigh;
        if ((up >> j) & 1) ++neigh;
        if (j < 3 && ((up >> (j + 1)) & 1)) ++neigh;
        /* down row */
        if (j > 0 && ((down >> (j - 1)) & 1)) ++neigh;
        if ((down >> j) & 1) ++neigh;
        if (j < 3 && ((down >> (j + 1)) & 1)) ++neigh;
        if (neigh != 2) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* 0 is outside the OEIS offset */

    /* Pre‑compute the validity table once */
    static bool init = false;
    static bool valid[16][16][16];
    if (!init) {
        for (int up = 0; up < 16; ++up)
            for (int mid = 0; mid < 16; ++mid)
                for (int down = 0; down < 16; ++down)
                    valid[up][mid][down] = is_valid_row(mid, up, down);
        init = true;
    }

    /* n = 1: only the empty row is valid */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int m = 0; m < 16; ++m)
            if (valid[0][m][0]) ans += (seq_t)1;
        return ans;
    }

    /* DP over pairs (previous row, current row) */
    seq_t dp_prev[256] = {0};
    seq_t dp_next[256];

    /* Initial pairs: first two rows, with an empty row above the first */
    for (int m0 = 0; m0 < 16; ++m0)
        for (int m1 = 0; m1 < 16; ++m1)
            if (valid[0][m0][m1])
                dp_prev[m0 * 16 + m1] = (seq_t)1;

    /* Process rows 3 … n (0‑based indexing) */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;
        for (int idx = 0; idx < 256; ++idx) {
            seq_t ways = dp_prev[idx];
            if (ways == (seq_t)0) continue;
            int prev = idx / 16;
            int curr = idx % 16;
            for (int nxt = 0; nxt < 16; ++nxt) {
                if (valid[prev][curr][nxt])
                    dp_next[curr * 16 + nxt] += ways;
            }
        }
        for (int i = 0; i < 256; ++i) dp_prev[i] = dp_next[i];
    }

    /* Final verification: last row has no row below */
    seq_t ans = (seq_t)0;
    for (int idx = 0; idx < 256; ++idx) {
        seq_t ways = dp_prev[idx];
        if (ways == (seq_t)0) continue;
        int prev = idx / 16;
        int last = idx % 16;
        if (valid[prev][last][0]) ans += ways;
    }
    return ans;
}
