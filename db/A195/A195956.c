/*
 * A195956  Number of n X 2 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors unequal to itself.
 * https://oeis.org/A195956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via dynamic programming (transfer‑matrix) for sequence
 *            A195956
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_CODES 25
#define NO_ROW 255

/* encode a row as a pair of values 0..4:
   code = 5 * left + right  (left = column 1, right = column 2) */

/* tables for row values */
static unsigned char row_left[MAX_CODES];
static unsigned char row_right[MAX_CODES];

/* pre‑computed validity tables */
static bool top_valid[MAX_CODES][MAX_CODES];
static bool bottom_valid[MAX_CODES][MAX_CODES];
static bool middle_valid[MAX_CODES][MAX_CODES][MAX_CODES];

/* list of next rows compatible with a (prev,curr) pair */
static unsigned char next_list[MAX_CODES][MAX_CODES][MAX_CODES];
static unsigned char next_cnt[MAX_CODES][MAX_CODES];

/* initialization flag */
static bool initialized = false;

/* check whether a row satisfies the neighbor constraint, given optional
   up and down rows (NO_ROW denotes missing row). */
static bool row_valid(int up_idx, int curr_idx, int down_idx)
{
    int a = row_left[curr_idx];
    int b = row_right[curr_idx];
    int up_a = (up_idx == NO_ROW) ? 0 : row_left[up_idx];
    int up_b = (up_idx == NO_ROW) ? 0 : row_right[up_idx];
    int down_a = (down_idx == NO_ROW) ? 0 : row_left[down_idx];
    int down_b = (down_idx == NO_ROW) ? 0 : row_right[down_idx];

    int cnt_a = (b != a);
    if (up_idx != NO_ROW) cnt_a += (up_a != a);
    if (down_idx != NO_ROW) cnt_a += (down_a != a);
    if (cnt_a != a) return false;

    int cnt_b = (a != b);
    if (up_idx != NO_ROW) cnt_b += (up_b != b);
    if (down_idx != NO_ROW) cnt_b += (down_b != b);
    if (cnt_b != b) return false;

    return true;
}

/* one‑time table construction */
static void construct_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* populate row value tables */
    for (int code = 0; code < MAX_CODES; ++code) {
        row_left[code] = code / 5;
        row_right[code] = code % 5;
    }

    /* compute top and bottom validity tables (single missing neighbour) */
    for (int curr = 0; curr < MAX_CODES; ++curr) {
        for (int next = 0; next < MAX_CODES; ++next) {
            top_valid[curr][next] = row_valid(NO_ROW, curr, next);
        }
    }
    for (int prev = 0; prev < MAX_CODES; ++prev) {
        for (int curr = 0; curr < MAX_CODES; ++curr) {
            bottom_valid[prev][curr] = row_valid(prev, curr, NO_ROW);
        }
    }

    /* compute middle validity and next lists */
    for (int prev = 0; prev < MAX_CODES; ++prev) {
        for (int curr = 0; curr < MAX_CODES; ++curr) {
            int cnt = 0;
            for (int next = 0; next < MAX_CODES; ++next) {
                if (row_valid(prev, curr, next)) {
                    middle_valid[prev][curr][next] = true;
                    next_list[prev][curr][cnt++] = (unsigned char)next;
                } else {
                    middle_valid[prev][curr][next] = false;
                }
            }
            next_cnt[prev][curr] = (unsigned char)cnt;
        }
    }
}

seq_t a(idx_t n)
{
    construct_tables();

    /* Special case n == 1: only one row, no vertical neighbours. */
    if (n == 1) {
        seq_t res = 0;
        for (int code = 0; code < MAX_CODES; ++code) {
            if (row_valid(NO_ROW, code, NO_ROW)) {
                res += 1;
            }
        }
        return res;
    }

    /* DP arrays: dp[prev][curr] – number of ways for the last processed
       pair of rows (prev = row_{i-1}, curr = row_i). */
    static seq_t dp[MAX_CODES][MAX_CODES];
    static seq_t dp_next[MAX_CODES][MAX_CODES];

    /* initialize dp with first two rows using top_valid */
    for (int i = 0; i < MAX_CODES; ++i)
        for (int j = 0; j < MAX_CODES; ++j)
            dp[i][j] = 0;

    for (int r1 = 0; r1 < MAX_CODES; ++r1) {
        for (int r2 = 0; r2 < MAX_CODES; ++r2) {
            if (top_valid[r1][r2]) {
                dp[r1][r2] += (seq_t)1;
            }
        }
    }

    /* Process rows 3 through n using transition lists */
    for (idx_t i = 3; i <= n; ++i) {
        /* reset dp_next */
        for (int a = 0; a < MAX_CODES; ++a)
            for (int b = 0; b < MAX_CODES; ++b)
                dp_next[a][b] = 0;

        for (int prev = 0; prev < MAX_CODES; ++prev) {
            for (int curr = 0; curr < MAX_CODES; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                unsigned char cnt = next_cnt[prev][curr];
                unsigned char *list = next_list[prev][curr];
                for (unsigned int k = 0; k < cnt; ++k) {
                    int next = list[k];
                    dp_next[curr][next] += ways;
                }
            }
        }
        /* swap dp and dp_next */
        for (int a = 0; a < MAX_CODES; ++a)
            for (int b = 0; b < MAX_CODES; ++b)
                dp[a][b] = dp_next[a][b];
    }

    /* Final step: enforce bottom constraints on the last row */
    seq_t result = 0;
    for (int prev = 0; prev < MAX_CODES; ++prev) {
        for (int curr = 0; curr < MAX_CODES; ++curr) {
            if (bottom_valid[prev][curr]) {
                result += dp[prev][curr];
            }
        }
    }
    return result;
}
