/*
 * A231539  Number of nX3 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A231539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = 20, a(3) = 116, a(4) = 589, a(5) = 3001, a(6) =
 *            15644, a(7) = 81179, a(8) = 420243, a(9) = 2177937, a(10) =
 *            11287977, a(11) = 58494710, a(12) = 303132340, a(13) =
 *            1570918959, a(14) = 8140881877, a(15) = 42188034539, a(16) =
 *            218628945511, a(17) = 1132989453013, a(18) = 58714320300...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool row_ok(uint8_t prev_row, uint8_t cur_row, uint8_t next_row,
                         bool has_prev, bool has_next)
{
    for (int c = 0; c < 3; ++c) {
        int val = (cur_row >> c) & 1;
        if (val == 1) continue;        /* a 1 is always valid */
        int neigh = 0, ones = 0;

        /* horizontal neighbours in the same row */
        if (c > 0) { neigh++; ones += (cur_row >> (c - 1)) & 1; }
        if (c < 2) { neigh++; ones += (cur_row >> (c + 1)) & 1; }

        /* vertical and anti‑diagonal neighbours from the row above */
        if (has_prev) {
            neigh++; ones += (prev_row >> c) & 1;           /* vertical up */
            if (c < 2) { neigh++; ones += (prev_row >> (c + 1)) & 1; }   /* up‑right */
        }

        /* vertical and anti‑diagonal neighbours from the row below */
        if (has_next) {
            neigh++; ones += (next_row >> c) & 1;           /* vertical down */
            if (c > 0) { neigh++; ones += (next_row >> (c - 1)) & 1; }   /* down‑left */
        }

        if (ones > neigh / 2) return false;  /* strict majority violated */
    }
    return true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Special case n = 0 (outside the offset) */
    if (n == 0) return (seq_t)0;

    /* For n = 1 we evaluate all 8 possible 3‑bit rows directly. */
    if (n == 1) {
        seq_t count = (seq_t)0;
        for (uint8_t row = 0; row < 8; ++row) {
            if (row_ok(0, row, 0, false, false)) count++;
        }
        return count;
    }

    /* DP arrays for the last two rows.   dp[prev][cur] holds the count
       of ways for a partial matrix ending with rows prev and cur. */
    seq_t dp[8][8] = {0};
    seq_t newdp[8][8];

    /* Initialise for the first two rows (row0 and row1). */
    for (uint8_t r0 = 0; r0 < 8; ++r0) {
        for (uint8_t r1 = 0; r1 < 8; ++r1) {
            if (row_ok(0, r0, r1, false, true)) {
                dp[r0][r1] += (seq_t)1;
            }
        }
    }

    /* Process rows 2 through n‑1 (0‑based indices). */
    for (idx_t i = 2; i < n; ++i) {
        /* Zero newdp */
        for (uint8_t a = 0; a < 8; ++a)
            for (uint8_t b = 0; b < 8; ++b)
                newdp[a][b] = (seq_t)0;

        for (uint8_t prev = 0; prev < 8; ++prev) {
            for (uint8_t cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == (seq_t)0) continue;
                for (uint8_t nxt = 0; nxt < 8; ++nxt) {
                    if (row_ok(prev, cur, nxt, true, true)) {
                        newdp[cur][nxt] += ways;
                    }
                }
            }
        }

        /* Copy newdp back to dp for the next iteration */
        for (uint8_t a = 0; a < 8; ++a)
            for (uint8_t b = 0; b < 8; ++b)
                dp[a][b] = newdp[a][b];
    }

    /* Sum over all possibilities where the last row is valid */
    seq_t total = (seq_t)0;
    for (uint8_t prev = 0; prev < 8; ++prev) {
        for (uint8_t cur = 0; cur < 8; ++cur) {
            seq_t ways = dp[prev][cur];
            if (ways == (seq_t)0) continue;
            if (row_ok(prev, cur, 0, true, false)) {
                total += ways;
            }
        }
    }

    return total;
}
