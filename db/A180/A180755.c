/*
 * A180755  Half the number of nX6 binary arrays with each element equal to at
 *          least two neighbors.
 * https://oeis.org/A180755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence counts half the number of n×6 binary arrays where
 *            each element has at least two equal neighbors. The algorithm
 *            uses dynamic programming over rows. It pre‑computes, once, for
 *            every pair (up,row) the set of possible next rows that keep row
 *            valid with its upper and lower neighbours, the...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define PAT 64                       /* number of possible 6‑bit rows */

/* Check if a single row is valid given whether the upper/lower neighbours
   exist and the bit patterns of these rows. */
static inline bool row_valid(uint8_t row,
                             bool up_present, uint8_t up_row,
                             bool down_present, uint8_t down_row)
{
    for (uint8_t c = 0; c < 6; ++c) {
        uint8_t cell = (row >> c) & 1;
        uint8_t eq = 0;

        if (c > 0 && cell == ((row >> (c - 1)) & 1)) ++eq;
        if (c < 5 && cell == ((row >> (c + 1)) & 1)) ++eq;
        if (up_present && cell == ((up_row >> c) & 1)) ++eq;
        if (down_present && cell == ((down_row >> c) & 1)) ++eq;

        if (eq < 2)
            return false;
    }
    return true;
}

/* Main sequence function.  It remains independent of the exact type of
   seq_t and idx_t; they are supplied by the build configuration. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 */

    /* Pre‑compute transition masks once.  allow_next[up][row] holds a
       bitmask: bit d is set iff adding row 'd' below (up,row) keeps
       'row' valid. */
    static uint64_t allow_next[PAT][PAT];
    static bool initialized = false;
    if (!initialized) {
        for (uint8_t up = 0; up < PAT; ++up) {
            for (uint8_t cur = 0; cur < PAT; ++cur) {
                uint64_t m = 0;
                for (uint8_t down = 0; down < PAT; ++down) {
                    if (row_valid(cur, true, up, true, down))
                        m |= 1ULL << down;
                }
                allow_next[up][cur] = m;
            }
        }
        initialized = true;
    }

    /* DP tables: dp[up][cur] holds the number of ways to reach the
       current (up,cur) pair.  ndp is used as temporary storage for the
       next row. */
    static seq_t dp[PAT][PAT];
    static seq_t ndp[PAT][PAT];

    /* Initialise dp to zero. */
    for (uint8_t i = 0; i < PAT; ++i)
        for (uint8_t j = 0; j < PAT; ++j)
            dp[i][j] = 0;

    /* Base: first two rows.  The first row must be valid with only a
       lower neighbour (the second row). */
    for (uint8_t first = 0; first < PAT; ++first)
        for (uint8_t second = 0; second < PAT; ++second)
            if (row_valid(first, false, 0, true, second))
                dp[first][second] = 1;

    /* Handle n == 2 separately (no middle rows). */
    if (n == 2) {
        seq_t total = 0;
        for (uint8_t prev = 0; prev < PAT; ++prev)
            for (uint8_t last = 0; last < PAT; ++last) {
                if (dp[prev][last] &&
                    row_valid(last, true, prev, false, 0))
                    total += dp[prev][last];
            }
        return total / 2;           /* half the arrays */
    }

    /* Process rows 3 … n.  After k rows we always hold the last two rows
       in dp[up][cur]. */
    for (idx_t row = 2; row < n; ++row) {        /* row counts from 0 */
        /* Clear ndp. */
        for (uint8_t i = 0; i < PAT; ++i)
            for (uint8_t j = 0; j < PAT; ++j)
                ndp[i][j] = 0;

        /* Extend each current state by allowed next rows. */
        for (uint8_t up = 0; up < PAT; ++up) {
            for (uint8_t cur = 0; cur < PAT; ++cur) {
                seq_t val = dp[up][cur];
                if (val == 0)
                    continue;
                uint64_t mask = allow_next[up][cur];
                for (uint8_t next = 0; next < PAT; ++next) {
                    if (mask & (1ULL << next))
                        ndp[cur][next] += val;
                }
            }
        }

        /* Move ndp → dp for next iteration. */
        for (uint8_t i = 0; i < PAT; ++i)
            for (uint8_t j = 0; j < PAT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row: must be valid with only an upper neighbour. */
    seq_t total = 0;
    for (uint8_t prev = 0; prev < PAT; ++prev)
        for (uint8_t last = 0; last < PAT; ++last) {
            if (dp[prev][last] &&
                row_valid(last, true, prev, false, 0))
                total += dp[prev][last];
        }

    return total / 2;               /* half the arrays */
}
