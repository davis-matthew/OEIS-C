/*
 * A183385  Half the number of nX6 binary arrays with no element equal to a
 *          strict majority of its king-move neighbors.
 * https://oeis.org/A183385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count n×6 binary arrays where no cell is a strict
 * majority of its king‑move neighbors, then return half that number.
 * The DP proceeds by keeping pairs of consecutive rows; a row can
 * be added only if it satisfies the constraint with its two neighbours.
 *
 *   a(1) = 5, a(2) = 37, … (see OEIS A183385)
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a row satisfies its constraints given the rows above
 * and below.  The flags have_above / has_below indicate whether those
 * rows are present; if false the corresponding row is ignored. */
static inline bool row_valid(uint8_t above, uint8_t mid, uint8_t below,
                             bool has_above, bool has_below)
{
    for (int j = 0; j < 6; ++j) {
        int mid_bit = (mid >> j) & 1;
        int neigh_cnt = 0;
        int same_cnt = 0;

        /* horizontal neighbours */
        if (j > 0) {
            ++neigh_cnt;
            same_cnt += (((mid >> (j - 1)) & 1) == mid_bit);
        }
        if (j < 5) {
            ++neigh_cnt;
            same_cnt += (((mid >> (j + 1)) & 1) == mid_bit);
        }

        /* vertical / diagonal neighbours from the row above */
        if (has_above) {
            if (j > 0) {
                ++neigh_cnt;
                same_cnt += (((above >> (j - 1)) & 1) == mid_bit);
            }
            ++neigh_cnt;                /* vertical neighbour (above, same col) */
            same_cnt += (((above >> j) & 1) == mid_bit);
            if (j < 5) {
                ++neigh_cnt;
                same_cnt += (((above >> (j + 1)) & 1) == mid_bit);
            }
        }

        /* vertical / diagonal neighbours from the row below */
        if (has_below) {
            if (j > 0) {
                ++neigh_cnt;
                same_cnt += (((below >> (j - 1)) & 1) == mid_bit);
            }
            ++neigh_cnt;                /* vertical neighbour (below, same col) */
            same_cnt += (((below >> j) & 1) == mid_bit);
            if (j < 5) {
                ++neigh_cnt;
                same_cnt += (((below >> (j + 1)) & 1) == mid_bit);
            }
        }

        /* strict majority rule: same_cnt must NOT be > neigh_cnt/2 */
        if (same_cnt * 2 > neigh_cnt)
            return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* pre‑computed tables – built once */
    static bool tables_ready = false;
    static uint64_t allowed_next[64][64];   /* mask of next rows that keep 'cur' valid */
    static bool first_ok[64][64];           /* row0 valid with row1 (no above) */
    static bool last_ok[64][64];            /* row (last) valid with prev (no below) */

    if (!tables_ready) {
        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                uint64_t mask = 0;
                /* precompute allowed next rows for interior rows */
                for (int nxt = 0; nxt < 64; ++nxt) {
                    if (row_valid((uint8_t)prev, (uint8_t)cur, (uint8_t)nxt,
                                  true, true))
                        mask |= 1ULL << nxt;
                }
                allowed_next[prev][cur] = mask;

                /* first row (no above, has below) */
                if (row_valid(0, (uint8_t)prev, (uint8_t)cur, false, true))
                    first_ok[prev][cur] = true;

                /* last row (has above, no below) */
                if (row_valid((uint8_t)prev, (uint8_t)cur, 0, true, false))
                    last_ok[prev][cur] = true;
            }
        }
        tables_ready = true;
    }

    if (n == 0) return 0;   /* undefined by offset, but harmless */

    /* n == 1 special case: single row, no neighbours above/below */
    if (n == 1) {
        seq_t cnt = 0;
        for (int mid = 0; mid < 64; ++mid)
            if (row_valid(0, (uint8_t)mid, 0, false, false))
                cnt += 1;
        return cnt / 2;   /* halve as per definition */
    }

    /* DP over row pairs */
    static seq_t dp[64][64];
    static seq_t nxt[64][64];

    /* clear dp */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* initialise with first two rows */
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            if (first_ok[r0][r1])
                dp[r0][r1] = 1;
        }
    }

    /* iterate through rows 2 .. n-1 (position index) */
    for (idx_t pos = 2; pos < n; ++pos) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                nxt[i][j] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                uint64_t mask = allowed_next[prev][cur];
                while (mask) {
                    int next_row = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    nxt[cur][next_row] += ways;
                }
            }
        }

        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* sum over final row pair satisfying last row condition */
    seq_t total = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            if (dp[prev][cur] && last_ok[prev][cur])
                total += dp[prev][cur];
        }
    }

    return total / 2;   /* halve as required */
}
