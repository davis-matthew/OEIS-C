/*
 * A183549  Number of nX4 0..2 arrays with each element equal to either the
 *          maximum or the minimum of its horizontal and vertical neighbors.
 * https://oeis.org/A183549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of number of n×4 0..2 arrays with each element equal to either the
 * maximum or the minimum of its horizontal and vertical neighbors. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint8_t rows_digits[81][4];
static uint8_t top_valid[81][81];
static uint8_t bottom_valid[81][81];
static uint8_t next_counts[81][81];
static uint8_t next_table[81][81][81];
static bool tables_computed = false;

static bool validate_row(const uint8_t *row, const uint8_t *up,
                         const uint8_t *down, bool up_exist,
                         bool down_exist)
{
    for (int j = 0; j < 4; ++j) {
        int mn = 3;   /* larger than any possible value */
        int mx = -1;  /* smaller than any possible value */

        if (up_exist) {
            int v = up[j];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        if (down_exist) {
            int v = down[j];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        if (j > 0) {
            int v = row[j - 1];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        if (j < 3) {
            int v = row[j + 1];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        int val = row[j];
        if (!(val == mn || val == mx))
            return false;
    }
    return true;
}

static void init_tables(void)
{
    if (tables_computed) return;

    /* enumerate all 3^4 rows */
    for (int i = 0; i < 81; ++i) {
        int t = i;
        for (int j = 0; j < 4; ++j) {
            rows_digits[i][j] = t % 3;
            t /= 3;
        }
    }

    for (int prev = 0; prev < 81; ++prev) {
        for (int curr = 0; curr < 81; ++curr) {
            top_valid[prev][curr] =
                validate_row(rows_digits[prev],
                             NULL, rows_digits[curr],
                             false, true);

            bottom_valid[prev][curr] =
                validate_row(rows_digits[curr],
                             rows_digits[prev], NULL,
                             true, false);

            int cnt = 0;
            for (int next = 0; next < 81; ++next) {
                if (validate_row(rows_digits[curr],
                                 rows_digits[prev], rows_digits[next],
                                 true, true)) {
                    next_table[prev][curr][cnt++] = (uint8_t)next;
                }
            }
            next_counts[prev][curr] = (uint8_t)cnt;
        }
    }

    tables_computed = true;
}

static seq_t count_single_row(void)
{
    seq_t ans = 0;
    for (int r = 0; r < 81; ++r) {
        if (validate_row(rows_digits[r], NULL, NULL,
                         false, false))
            ans += 1;
    }
    return ans;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;
    if (n == 1) return count_single_row();

    seq_t dp[81][81];
    seq_t ndp[81][81];

    /* initialise dp for first two rows */
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            dp[i][j] = 0;
    for (int prev = 0; prev < 81; ++prev) {
        for (int curr = 0; curr < 81; ++curr) {
            if (top_valid[prev][curr])
                dp[prev][curr] = 1;
        }
    }

    /* process rows 3 ... n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 81; ++prev) {
            for (int curr = 0; curr < 81; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                uint8_t c = next_counts[prev][curr];
                const uint8_t *list = next_table[prev][curr];
                for (int k = 0; k < c; ++k) {
                    uint8_t next = list[k];
                    ndp[curr][next] += ways;
                }
            }
        }

        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row must be validated against its upper neighbour */
    seq_t ans = 0;
    for (int prev = 0; prev < 81; ++prev) {
        for (int curr = 0; curr < 81; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways) continue;
            if (bottom_valid[prev][curr])
                ans += ways;
        }
    }
    return ans;
}
