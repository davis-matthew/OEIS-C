/*
 * A231803  Number of (n+1)X(5+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to one.
 * https://oeis.org/A231803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A231803: number of (n+1)×6 binary arrays with no
   element having a strict majority of its 8-connected neighbours equal
   to one.  The function uses dynamic programming over rows. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COLS   6
#define STATES (1 << COLS)      /* 64 */

static inline bool row_ok(uint8_t curr, uint8_t prev, uint8_t next,
                          bool has_prev, bool has_next)
{
    /* Checks that all neighbours of the cells of 'curr' satisfy the
       strict majority rule, given neighbours from 'prev' and 'next'.
       `has_prev` / `has_next` are true if the neighbouring rows exist. */
    for (int j = 0; j < COLS; ++j) {
        int ones = 0, tot = 0;

        /* neighbours on same row */
        if (j > 0) { ones += (curr >> (j - 1)) & 1; tot++; }
        if (j < COLS - 1) { ones += (curr >> (j + 1)) & 1; tot++; }

        /* neighbours on previous row */
        if (has_prev) {
            if (j > 0) { ones += (prev >> (j - 1)) & 1; tot++; }
            ones += (prev >> j) & 1; tot++;
            if (j < COLS - 1) { ones += (prev >> (j + 1)) & 1; tot++; }
        }

        /* neighbours on next row */
        if (has_next) {
            if (j > 0) { ones += (next >> (j - 1)) & 1; tot++; }
            ones += (next >> j) & 1; tot++;
            if (j < COLS - 1) { ones += (next >> (j + 1)) & 1; tot++; }
        }

        if (ones * 2 > tot) return false;   /* strict majority of 1 */
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Precomputation of transition masks and first/last row checks. */
    static unsigned long long validMask[STATES][STATES];   /* next masks */
    static unsigned long long firstMask[STATES];           /* for first row */
    static bool lastValid[STATES][STATES];                /* last row ok? */
    static bool prepared = false;

    if (!prepared) {
        /* ValidMask: for each (prev, curr) pair, a bitmask of next rows
           that make 'curr' valid when all three rows are present. */
        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                unsigned long long mask = 0ULL;
                for (int next = 0; next < STATES; ++next) {
                    if (row_ok((uint8_t)curr, (uint8_t)prev, (uint8_t)next,
                               true, true))
                        mask |= (1ULL << next);
                }
                validMask[prev][curr] = mask;
            }
        }

        /* firstMask: for each first row pattern, bitmask of second rows
           that keep first row valid (no upper neighbours). */
        for (int first = 0; first < STATES; ++first) {
            unsigned long long mask = 0ULL;
            for (int second = 0; second < STATES; ++second) {
                if (row_ok((uint8_t)first, 0, (uint8_t)second,
                           false, true))
                    mask |= (1ULL << second);
            }
            firstMask[first] = mask;
        }

        /* lastValid: whether a pair (prev, curr) satisfies the last row
           (no lower neighbours). */
        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                lastValid[prev][curr] = row_ok((uint8_t)curr,
                                               (uint8_t)prev, 0,
                                               true, false);
            }
        }
        prepared = true;
    }

    /* DP arrays: dp[prev][curr] holds counts for the last two rows processed. */
    seq_t dp[STATES][STATES];
    seq_t ndp[STATES][STATES];
    memset(dp, 0, sizeof(dp));
    memset(ndp, 0, sizeof(ndp));

    /* Initialize with all possible first and second rows. */
    for (int first = 0; first < STATES; ++first) {
        unsigned long long mask = firstMask[first];
        while (mask) {
            int second = __builtin_ctzll(mask);
            mask &= mask - 1;
            dp[first][second] = 1;
        }
    }

    /* Number of rows to process: total rows = n+1. We already have 2 rows. */
    int total_rows = (int)(n + 1);
    for (int step = 2; step < total_rows; ++step) {
        /* Transition to the next row. */
        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                unsigned long long mask = validMask[prev][curr];
                while (mask) {
                    int next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    ndp[curr][next] += val;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
        memset(ndp, 0, sizeof(ndp));
    }

    /* Sum results for valid final rows. */
    seq_t result = 0;
    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            if (dp[prev][curr] && lastValid[prev][curr])
                result += dp[prev][curr];
        }
    }

    return result;
}
