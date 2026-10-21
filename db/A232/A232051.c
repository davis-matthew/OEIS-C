/*
 * A232051  Number of 5Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over columns. a(n) = number of 5
 *            × n binary arrays in which no element is less than a strict
 *            majority of its horizontal and antidiagonal neighbors
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each element must not have more 1‑neighbors than its own value 0. */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define NULLCOL 32          /* 0..31 are normal 5‑bit column patterns, 32 denotes missing left column */
#define COLPAT 32           /* 5 bits -> 32 possible column patterns */

/* Test validity of a single column when it has the given left and right neighbours.
   left_exist/right_exist must be 0 or 1.  left_col/right_col meaningful only if the neighbour exists. */
static bool col_valid(uint8_t col, int left_exist, uint8_t left_col,
                      int right_exist, uint8_t right_col)
{
    for (int r = 0; r < 5; ++r) {
        int neigh_cnt = 0;
        int ones      = 0;

        /* horizontal neighbours */
        if (left_exist) { ++neigh_cnt; if (left_col >> r & 1) ++ones; }
        if (right_exist) { ++neigh_cnt; if (right_col >> r & 1) ++ones; }

        /* antidiagonal neighbours (NW‑SE direction) */
        if (left_exist && r > 0) { ++neigh_cnt; if (left_col >> (r - 1) & 1) ++ones; }
        if (right_exist && r < 4) { ++neigh_cnt; if (right_col >> (r + 1) & 1) ++ones; }

        /* a 0 cell must not have a strict majority of 1‑neighbours */
        if ((col >> r & 1) == 0) {
            if (ones > neigh_cnt / 2) return false;
        }
    }
    return true;
}

/* Pre‑computed tables:
   valid[prev][curr][next]  : true if column 'curr' is valid with
                              left neighbour 'prev' (or none if prev==NULLCOL)
                              and right neighbour 'next'.
   final_valid[prev][curr] : true if column 'curr' is valid with
                            left neighbour 'prev' and no right neighbour. */
static bool valid[NULLCOL + 1][COLPAT][COLPAT];
static bool final_valid[NULLCOL + 1][COLPAT];

static void precompute_tables(void)
{
    for (int prev = 0; prev <= NULLCOL; ++prev) {
        int left_exist = (prev != NULLCOL);
        uint8_t left_col = (prev == NULLCOL ? 0 : (uint8_t)prev);
        for (int curr = 0; curr < COLPAT; ++curr) {
            for (int next = 0; next < COLPAT; ++next) {
                valid[prev][curr][next] =
                    col_valid((uint8_t)curr, left_exist, left_col,
                              1, (uint8_t)next);
            }
            if (prev == NULLCOL)
                final_valid[prev][curr] = true; /* no left neighbour, no neighbours at all */
            else
                final_valid[prev][curr] =
                    col_valid((uint8_t)curr, 1, (uint8_t)prev,
                              0, 0);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used */

    static seq_t dp[NULLCOL + 1][COLPAT];
    static seq_t ndp[NULLCOL + 1][COLPAT];
    static bool tables_built = false;
    if (!tables_built) {
        precompute_tables();
        tables_built = true;
    }

    /* Initialise for the first column: left neighbour missing */
    memset(dp, 0, sizeof(dp));
    for (int p = 0; p < COLPAT; ++p)
        dp[NULLCOL][p] = (seq_t)1;

    /* Process columns 2..n */
    for (idx_t col_idx = 2; col_idx <= n; ++col_idx) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev <= NULLCOL; ++prev) {
            for (int curr = 0; curr < COLPAT; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < COLPAT; ++next) {
                    if (valid[prev][curr][next]) {
                        ndp[curr][next] += ways;
                    }
                }
            }
        }
        /* swap dp and ndp by copying */
        for (int i = 0; i <= NULLCOL; ++i)
            memcpy(dp[i], ndp[i], sizeof(seq_t) * COLPAT);
    }

    /* Final column: no right neighbour */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev <= NULLCOL; ++prev) {
        for (int curr = 0; curr < COLPAT; ++curr) {
            if (dp[prev][curr] && final_valid[prev][curr])
                result += dp[prev][curr];
        }
    }
    return result;
}
