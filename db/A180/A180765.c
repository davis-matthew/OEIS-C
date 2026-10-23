/*
 * A180765  Half the number of nX5 binary arrays with each element equal to at
 *          least one neighbor.
 * https://oeis.org/A180765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180765: Half the number of n×5 binary arrays with each element equal
 * to at least one orthogonal neighbor (left, right, above, or below). */
#include <stdint.h>
#include <stdbool.h>

/* Check whether every cell in 'row' has at least one equal orthogonal neighbor.
 * 'above' and 'below' are the rows immediately above and below, or -1 if none.
 */
static bool row_ok(uint8_t row, int above, int below)
{
    for (int j = 0; j < 5; ++j) {
        uint8_t val = (row >> j) & 1;
        bool neigh = false;

        /* horizontal neighbors in the same row */
        if (j > 0   && ((row >> (j - 1)) & 1) == val) neigh = true;
        if (j < 4   && ((row >> (j + 1)) & 1) == val) neigh = true;

        /* vertical neighbors */
        if (above >= 0 && ((above >> j) & 1) == val) neigh = true;
        if (below >= 0 && ((below >> j) & 1) == val) neigh = true;

        if (!neigh) return false;
    }
    return true;
}

/* Precompute for each (prev, curr) pair the list of next rows that keep the
 * current row satisfied.
 */
static bool ready = false;
static uint8_t next_list[32][32][32];
static uint8_t next_count[32][32];

static void precompute(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            int cnt = 0;
            for (int next = 0; next < 32; ++next) {
                if (row_ok((uint8_t)curr, prev, next)) {
                    next_list[prev][curr][cnt++] = (uint8_t)next;
                }
            }
            next_count[prev][curr] = (uint8_t)cnt;
        }
    }
    ready = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Termination for undefined index */
    if (n == 0) return 0;

    if (!ready) precompute();

    /* Handle a(1) directly: count rows, then half */
    if (n == 1) {
        seq_t cnt = 0;
        for (int row = 0; row < 32; ++row) {
            if (row_ok((uint8_t)row, -1, -1))
                ++cnt;
        }
        return cnt >> 1;   /* divide by 2 */
    }

    /* DP arrays: dp[cur][prev][curr] */
    static seq_t dp[2][32][32];
    int cur = 0, nxt = 1;

    /* Zero current layer */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[cur][i][j] = 0;

    /* Initialize with first two rows */
    for (int r1 = 0; r1 < 32; ++r1) {
        for (int r2 = 0; r2 < 32; ++r2) {
            if (row_ok((uint8_t)r1, -1, r2))
                ++dp[cur][r1][r2];
        }
    }

    idx_t rows_done = 2;
    while (rows_done < n) {
        /* Zero next layer */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[nxt][i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[cur][prev][curr];
                if (!ways) continue;
                uint8_t cnt = next_count[prev][curr];
                for (int k = 0; k < cnt; ++k) {
                    uint8_t next = next_list[prev][curr][k];
                    dp[nxt][curr][next] += ways;
                }
            }
        }

        /* swap layers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
        ++rows_done;
    }

    /* Sum over all valid endings: last row must satisfy rule with no below neighbor */
    seq_t total = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int last = 0; last < 32; ++last) {
            if (row_ok((uint8_t)last, prev, -1))
                total += dp[cur][prev][last];
        }
    }

    return total >> 1;   /* divide by 2 */
}
