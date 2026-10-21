/*
 * A202790  Number of n X 3 binary arrays with every one adjacent to another
 *          one horizontally, diagonally, antidiagonally or vertically.
 * https://oeis.org/A202790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays where every 1 has at least *
 *            one adjacent 1 (horizontally, vertically, diagonally or
 *            anti‐diagonally). * * The values are computed by dynamic
 *            programming over rows, keeping for * each row a 3‑bit mask of
 *            which cells contain 1 and a 3‑bit pending mask * of those...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const uint8_t neighbor_cols[3] = {
    (1 << 0) | (1 << 1),   /* column 0 is adjacent vertically or diagonally to col 0,1 */
    (1 << 0) | (1 << 1) | (1 << 2), /* column 1 to 0,1,2 */
    (1 << 1) | (1 << 2)    /* column 2 to 1,2 */
};

static bool precomp_done = false;
static uint8_t init_pending[8];
static uint8_t sat_prev_bits[8][8];
static uint8_t pending_curr_mask[8][8];

static void init_tables(void) {
    if (precomp_done) return;
    precomp_done = true;

    for (int mask = 0; mask < 8; ++mask) {
        uint8_t pend = 0;
        for (int j = 0; j < 3; ++j) {
            if ((mask & (1 << j)) == 0) continue;
            bool has_adj = false;
            if (j > 0 && (mask & (1 << (j - 1)))) has_adj = true;
            if (j < 2 && (mask & (1 << (j + 1)))) has_adj = true;
            if (!has_adj) pend |= 1 << j;
        }
        init_pending[mask] = pend;
    }

    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            uint8_t sat = 0;
            for (int j = 0; j < 3; ++j) {
                if ((prev & (1 << j)) && (curr & neighbor_cols[j]))
                    sat |= 1 << j;
            }
            sat_prev_bits[prev][curr] = sat;

            uint8_t pend_curr = 0;
            for (int j = 0; j < 3; ++j) {
                if ((curr & (1 << j)) == 0) continue;
                bool satisfied = false;
                /* horizontal adjacency within the current row */
                if (j > 0 && (curr & (1 << (j - 1)))) satisfied = true;
                if (j < 2 && (curr & (1 << (j + 1)))) satisfied = true;
                /* adjacency to the previous row */
                if (!satisfied && (prev & neighbor_cols[j])) satisfied = true;
                if (!satisfied) pend_curr |= 1 << j;
            }
            pending_curr_mask[prev][curr] = pend_curr;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    init_tables();

    seq_t dp[8][8] = {0};
    seq_t nxt[8][8] = {0};

    /* initial row (row 0) */
    for (int mask = 0; mask < 8; ++mask)
        dp[mask][init_pending[mask]] += 1;

    for (idx_t row = 1; row < n; ++row) {
        /* zero next array */
        for (int m = 0; m < 8; ++m)
            for (int p = 0; p < 8; ++p)
                nxt[m][p] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int pre_pend = 0; pre_pend < 8; ++pre_pend) {
                seq_t val = dp[prev][pre_pend];
                if (val == 0) continue;

                for (int curr = 0; curr < 8; ++curr) {
                    /* check that all pending cells of previous row get a neighbour */
                    if ((pre_pend & ~sat_prev_bits[prev][curr]) != 0) continue;
                    uint8_t new_pend = pending_curr_mask[prev][curr];
                    nxt[curr][new_pend] += val;
                }
            }
        }
        /* copy nxt to dp for next iteration */
        for (int m = 0; m < 8; ++m)
            for (int p = 0; p < 8; ++p)
                dp[m][p] = nxt[m][p];
    }

    /* sum over states with no pending cells */
    seq_t total = 0;
    for (int mask = 0; mask < 8; ++mask)
        total += dp[mask][0];
    return total;
}
