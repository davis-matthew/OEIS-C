/*
 * A180756  Half the number of nX7 binary arrays with each element equal to at
 *          least two neighbors.
 * https://oeis.org/A180756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = half the number of n×7 binary arrays in which each
 *            element equals at least two of its four orthogonal neighbors
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 7
#define STATE_BITS 7
#define STATE_COUNT (1 << STATE_BITS)

/* Lookup tables */
static bool valid_first_row[STATE_COUNT][STATE_COUNT];
static bool valid_last_row[STATE_COUNT][STATE_COUNT];
static uint64_t next_mask[STATE_COUNT][STATE_COUNT][2];

/* Check row validity when there is no upper neighbor (used for first row). */
static inline bool valid_no_up(uint8_t row, uint8_t below)
{
    for (int j = 0; j < ROWS; ++j) {
        uint8_t v = (row >> j) & 1;
        int c = 0;
        if (j > 0 && ((row >> (j - 1)) & 1) == v) ++c;
        if (j < ROWS - 1 && ((row >> (j + 1)) & 1) == v) ++c;
        if (((below >> j) & 1) == v) ++c;            /* below neighbor */
        if (c < 2) return false;
    }
    return true;
}

/* Check row validity when there is no lower neighbor (used for last row). */
static inline bool valid_no_down(uint8_t row, uint8_t above)
{
    for (int j = 0; j < ROWS; ++j) {
        uint8_t v = (row >> j) & 1;
        int c = 0;
        if (j > 0 && ((row >> (j - 1)) & 1) == v) ++c;
        if (j < ROWS - 1 && ((row >> (j + 1)) & 1) == v) ++c;
        if (((above >> j) & 1) == v) ++c;            /* above neighbor */
        if (c < 2) return false;
    }
    return true;
}

/* Check row validity with both upper and lower neighbors (used for middle rows). */
static inline bool valid_middle(uint8_t row, uint8_t above, uint8_t below)
{
    for (int j = 0; j < ROWS; ++j) {
        uint8_t v = (row >> j) & 1;
        int c = 0;
        if (j > 0 && ((row >> (j - 1)) & 1) == v) ++c;
        if (j < ROWS - 1 && ((row >> (j + 1)) & 1) == v) ++c;
        if (((above >> j) & 1) == v) ++c;            /* above neighbor */
        if (((below >> j) & 1) == v) ++c;            /* below neighbor */
        if (c < 2) return false;
    }
    return true;
}

/* One‑time precomputation of tables */
static void init_tables(void)
{
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    for (int r1 = 0; r1 < STATE_COUNT; ++r1) {
        for (int r2 = 0; r2 < STATE_COUNT; ++r2) {
            valid_first_row[r1][r2] = valid_no_up((uint8_t)r1, (uint8_t)r2);
            valid_last_row[r2][r1]  = valid_no_down((uint8_t)r2, (uint8_t)r1);
        }
    }

    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        for (int curr = 0; curr < STATE_COUNT; ++curr) {
            next_mask[prev][curr][0] = 0ULL;
            next_mask[prev][curr][1] = 0ULL;
            for (int nxt = 0; nxt < STATE_COUNT; ++nxt) {
                if (valid_middle((uint8_t)curr, (uint8_t)prev, (uint8_t)nxt)) {
                    if (nxt < 64)
                        next_mask[prev][curr][0] |= 1ULL << nxt;
                    else
                        next_mask[prev][curr][1] |= 1ULL << (nxt - 64);
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    if (n <= 1)           /* a(1) = 0, a(0) unused */
        return 0;

    /* DP arrays (fixed size 128×128 = 16384). */
    static seq_t dp[STATE_COUNT * STATE_COUNT];
    static seq_t next[STATE_COUNT * STATE_COUNT];
    memset(dp, 0, sizeof dp);
    memset(next, 0, sizeof next);

    /* Initial pairs of first two rows.  r1 is already validated against r2. */
    for (unsigned r1 = 0; r1 < STATE_COUNT; ++r1) {
        for (unsigned r2 = 0; r2 < STATE_COUNT; ++r2) {
            if (valid_first_row[r1][r2]) {
                dp[r1 * STATE_COUNT + r2] = 1;
            }
        }
    }

    /* Process rows 3..n. */
    for (idx_t row = 3; row <= n; ++row) {
        memset(next, 0, sizeof next);
        for (unsigned prev = 0; prev < STATE_COUNT; ++prev) {
            for (unsigned curr = 0; curr < STATE_COUNT; ++curr) {
                seq_t val = dp[prev * STATE_COUNT + curr];
                if (!val) continue;
                /* iterate over permissable next rows */
                for (int w = 0; w < 2; ++w) {
                    uint64_t bits = next_mask[prev][curr][w];
                    while (bits) {
                        int t = __builtin_ctzll(bits);
                        int nxt = w * 64 + t;
                        bits &= bits - 1;
                        next[curr * STATE_COUNT + nxt] += val;
                    }
                }
            }
        }
        memcpy(dp, next, sizeof dp);
    }

    /* Sum over final states: last row must be valid with its upper neighbor. */
    seq_t total = 0;
    for (unsigned prev = 0; prev < STATE_COUNT; ++prev) {
        for (unsigned curr = 0; curr < STATE_COUNT; ++curr) {
            if (valid_last_row[curr][prev]) {
                total += dp[prev * STATE_COUNT + curr];
            }
        }
    }
    return total / 2;   /* the sequence is defined as half the count */
}
