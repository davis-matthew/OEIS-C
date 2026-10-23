/*
 * A183453  Number of nX6 binary arrays with every 1 having exactly three
 *          king-move neighbors equal to 1.
 * https://oeis.org/A183453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS sequence A183453:
 * Number of n×6 binary arrays in which every 1 has exactly
 * three king‑move neighbours that are 1.  The array is of size
 * n rows by 6 columns.  The algorithm performs dynamic programming
 * over rows with a 3‑row state.  A transition table for a
 * 6‑bit row pair is pre‑computed once.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Return true iff every 1 in the middle row has exactly three
 * neighbouring 1s in the 3×3 block formed by (prev, curr, next). */
static inline bool middle_ok(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        if (!(curr & (1u << j)))
            continue;               /* no constraint for a 0 */
        int cnt = 0;

        /* neighbours in previous row */
        if (j > 0 && (prev & (1u << (j - 1)))) ++cnt;
        if (prev & (1u << j))                 ++cnt;
        if (j < 5 && (prev & (1u << (j + 1)))) ++cnt;

        /* neighbours in current row (excluding the cell itself) */
        if (j > 0 && (curr & (1u << (j - 1)))) ++cnt;
        if (j < 5 && (curr & (1u << (j + 1)))) ++cnt;

        /* neighbours in next row */
        if (j > 0 && (next & (1u << (j - 1)))) ++cnt;
        if (next & (1u << j))                 ++cnt;
        if (j < 5 && (next & (1u << (j + 1)))) ++cnt;

        if (cnt != 3)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Transition table: trans[prev][curr] is a 64‑bit mask of
     * allowed next rows for the pair (prev, curr). */
    static uint64_t trans[64][64];
    static bool init = false;
    if (!init) {
        init = true;
        for (uint8_t prev = 0; prev < 64; ++prev) {
            for (uint8_t curr = 0; curr < 64; ++curr) {
                uint64_t mask = 0;
                for (uint8_t nxt = 0; nxt < 64; ++nxt) {
                    if (middle_ok(prev, curr, nxt))
                        mask |= 1ULL << nxt;
                }
                trans[prev][curr] = mask;
            }
        }
    }

    /* DP arrays: dp[prev][curr] = number of ways to reach this
     * state after processing the current row.  dpNext is used
     * for the next iteration. */
    static seq_t dp[64][64];
    static seq_t dpNext[64][64];
    memset(dp, 0, sizeof(dp));

    /* First row: pretend row 0 is all zeros. */
    for (uint8_t curr = 0; curr < 64; ++curr)
        dp[0][curr] = 1;

    if (n == 1) {
        /* Final state: the row after the last is zero. */
        seq_t ans = 0;
        for (uint8_t curr = 0; curr < 64; ++curr) {
            if (trans[0][curr] & 1ULL)   /* next = 0 allowed */
                ans += dp[0][curr];
        }
        return ans;
    }

    /* Process rows 2 .. n.  After row r, 'prev' holds row r‑2
     * and 'curr' holds row r‑1.  We transition to a new pair
     * (curr, nxt). */
    for (idx_t row = 2; row <= n; ++row) {
        memset(dpNext, 0, sizeof(dpNext));
        for (uint8_t prev = 0; prev < 64; ++prev) {
            for (uint8_t curr = 0; curr < 64; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways)
                    continue;
                uint64_t allowed = trans[prev][curr];
                if (!allowed)
                    continue;
                /* Iterate over set bits of the mask. */
                uint64_t bits = allowed;
                while (bits) {
                    int nxt = __builtin_ctzll(bits);
                    dpNext[curr][nxt] += ways;
                    bits &= bits - 1;       /* clear lowest set bit */
                }
            }
        }
        memcpy(dp, dpNext, sizeof(dp));
    }

    /* Final row: require the row after the last to be zero. */
    seq_t result = 0;
    for (uint8_t prev = 0; prev < 64; ++prev) {
        for (uint8_t curr = 0; curr < 64; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways)
                continue;
            if (trans[prev][curr] & 1ULL)   /* next = 0 allowed */
                result += ways;
        }
    }
    return result;
}
