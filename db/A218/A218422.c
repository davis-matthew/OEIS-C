/*
 * A218422  Number of n X 4 binary arrays with every 0 a horizontal, vertical
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A218422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is computed with a 2‑row transfer‑matrix DP (n≥1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The transition table enforces that every 0 cell has a horizontal, vertical
   or anti‑diagonal neighbour that is a 1.  The DP tracks the last two
   rows: prior (pri) and previous (prev), and adds a new row (cur). */

#include <stdint.h>
#include <stdbool.h>

/* Mask for a left neighbour (j-1) and a right neighbour (j+1). */
static const uint8_t left_mask[4]  = {0, 1 << 0, 1 << 1, 1 << 2};
static const uint8_t right_mask[4] = {1 << 1, 1 << 2, 1 << 3, 0};

/* Transition tables. */
static bool trans[16][16][16];   /* trans[pri][prev][cur] == true if all zeros
                                   in 'prev' are satisfied by neighbours in
                                   'pri' (above) or 'cur' (below). */
static bool final_ok[16][16];    /* true if zeros in 'prev' are satisfied
                                   by neighbours in 'pri' or horizontal
                                   neighbours in 'prev' only (no row below). */
static bool init_done = false;

/* Build the transition tables once. */
static void init_tables(void)
{
    for (int pri = 0; pri < 16; ++pri) {
        for (int prev = 0; prev < 16; ++prev) {
            /* Final‑row validity: zeros in 'prev' must have a neighbour
               in 'pri' (above) or horizontally in 'prev' only. */
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                if ((prev >> j) & 1) continue;                /* cell is 1 */
                if (prev & (left_mask[j] | right_mask[j])) continue;
                if (pri & (1 << j)) continue;                /* vertical above */
                if (pri & right_mask[j]) continue;            /* up‑right anti‑diag */
                ok = false;
                break;
            }
            final_ok[pri][prev] = ok;

            /* Transition: zeros in 'prev' must have a neighbour in
               'pri', 'cur', or horizontally within 'prev'. */
            for (int cur = 0; cur < 16; ++cur) {
                bool good = true;
                for (int j = 0; j < 4; ++j) {
                    if ((prev >> j) & 1) continue;
                    if (prev & (left_mask[j] | right_mask[j])) continue;
                    if (pri & (1 << j)) continue;                /* vertical above */
                    if (pri & right_mask[j]) continue;            /* up‑right anti‑diag */
                    if (cur & (1 << j)) continue;                /* vertical below */
                    if (cur & left_mask[j]) continue;             /* down‑left anti‑diag */
                    good = false;
                    break;
                }
                trans[pri][prev][cur] = good;
            }
        }
    }
    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!init_done) init_tables();

    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* Clear dp */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* First row: no row above; use 0 as the empty row. */
    for (int p = 0; p < 16; ++p)
        dp[0][p] = 1;

    if (n == 1) {
        seq_t total = 0;
        for (int p = 0; p < 16; ++p)
            if (final_ok[0][p])
                total += dp[0][p];
        return total;
    }

    /* Process subsequent rows. */
    for (idx_t row = 1; row < n; ++row) {
        /* Clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int pri = 0; pri < 16; ++pri) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t curcnt = dp[pri][prev];
                if (!curcnt) continue;
                for (int cur = 0; cur < 16; ++cur)
                    if (trans[pri][prev][cur])
                        ndp[prev][cur] += curcnt;
            }
        }

        /* Copy ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* After the last row, check that zeros in the final row are satisfied. */
    seq_t total = 0;
    for (int pri = 0; pri < 16; ++pri) {
        for (int prev = 0; prev < 16; ++prev) {
            if (final_ok[pri][prev])
                total += dp[pri][prev];
        }
    }
    return total;
}
