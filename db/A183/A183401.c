/*
 * A183401  Half the number of nX5 binary arrays with no element equal to a
 *          strict majority of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes half the number of n×5 binary arrays with no element equal to a
 * strict majority of its diagonal and antidiagonal neighbours.
 * This implementation uses a transfer‑matrix dynamic programming
 * over rows of width 5.  The DP runs in O(n·2⁵·2⁵) time, easily
 * fast enough for n up to about 10 000. */

#include <stdint.h>
#include <stdbool.h>

/* Helper to test a middle row against its upper and lower neighbours.
 * 'above' and 'below' are 5‑bit patterns.  'above_exists' / 'below_exists'
 * indicate whether those rows are present. */
static bool check_row(uint8_t row, uint8_t above, bool above_exists,
                      uint8_t below, bool below_exists) {
    for (int c = 0; c < 5; ++c) {
        int v = (row >> c) & 1;
        int total = 0, matches = 0;

        if (above_exists) {
            if (c > 0) {
                int nb = (above >> (c-1)) & 1;
                ++total; if (nb == v) ++matches;
            }
            if (c < 4) {
                int nb = (above >> (c+1)) & 1;
                ++total; if (nb == v) ++matches;
            }
        }
        if (below_exists) {
            if (c > 0) {
                int nb = (below >> (c-1)) & 1;
                ++total; if (nb == v) ++matches;
            }
            if (c < 4) {
                int nb = (below >> (c+1)) & 1;
                ++total; if (nb == v) ++matches;
            }
        }
        if (matches > total / 2) return false;
    }
    return true;
}

static bool valid[33][32][32];   /* pre‑computed transition table */

/* Precompute the transition table:
 * valid[prev][curr][next] == true  ⇔  curr satisfies its constraint
 * when its neighbours are 'prev' (or none if prev==32) and 'next'. */
static void build_transition_table(void) {
    for (int prev = 0; prev <= 32; ++prev) {
        bool prev_exists = (prev != 32);
        uint8_t above = prev;      /* value ignored if not exists */
        for (int curr = 0; curr < 32; ++curr) {
            for (int next = 0; next < 32; ++next) {
                uint8_t below = next;   /* below always exists in transitions */
                valid[prev][curr][next] = check_row((uint8_t)curr, above, prev_exists,
                                                    below, true);
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    /* Build transition table once */
    static bool table_built = false;
    if (!table_built) {
        build_transition_table();
        table_built = true;
    }

    /* DP arrays: cur[prev][curr], next[prev][curr] */
    static seq_t cur[33][32];
    static seq_t next[33][32];
    /* Initialise all to 0 */
    for (int p = 0; p <= 32; ++p)
        for (int c = 0; c < 32; ++c)
            cur[p][c] = 0, next[p][c] = 0;

    /* First row: previous row is "none" (index 32) */
    for (int row = 0; row < 32; ++row)
        cur[32][row] = 1;

    /* Process rows 2 … n */
    for (idx_t r = 2; r <= n; ++r) {
        /* clear next */
        for (int p = 0; p <= 32; ++p)
            for (int c = 0; c < 32; ++c)
                next[p][c] = 0;

        for (int prev = 0; prev <= 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = cur[prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (valid[prev][curr][nxt]) {
                        next[curr][nxt] += ways;
                    }
                }
            }
        }
        /* swap cur and next */
        for (int p = 0; p <= 32; ++p)
            for (int c = 0; c < 32; ++c)
                cur[p][c] = next[p][c];
    }

    /* Final step: check constraints for the bottom row (no below row) */
    seq_t total = 0;
    for (int prev = 0; prev <= 32; ++prev) {
        bool prev_exists = (prev != 32);
        for (int curr = 0; curr < 32; ++curr) {
            seq_t ways = cur[prev][curr];
            if (!ways) continue;
            if (check_row((uint8_t)curr,
                          prev, prev_exists,
                          0, false)) {
                total += ways;
            }
        }
    }

    /* The sequence is half the total number of arrays */
    return total / 2;
}
