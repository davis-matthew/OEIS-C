/*
 * A231540  Number of nX4 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A231540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   row_valid_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A231540 – Number of n×4 0..1 arrays with no element less than a strict majority
 *  of its horizontal, vertical and antidiagonal neighbours.
 *  The function computes the values by a straightforward O(n) dynamic program
 *  with a 16×16 state table (previous row, current row).  All intermediate
 *  arithmetic uses the seq_t type supplied by the build environment.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/row_valid_single.h"

/* Helper tables for row validity */
static bool valid_single[16];
static bool valid_first[16][16];
static bool valid_last[16][16];
static bool valid_internal[16][16][16];
static bool tables_ready = false;

/* General neighbour‑checking routine.  For each neighbour the flag
 * hasPrev/hasNext indicates whether that neighbour set exists.  The row
 * values prev and next may be arbitrary when the corresponding flag is
 * false. */
static bool row_valid(int prev, int curr, int next,
                      bool hasPrev, bool hasNext)
{
    for (int j = 0; j < 4; ++j)
    {
        if ((curr >> j) & 1)            /* a 1 is always fine           */
            continue;

        int ones = 0, k = 0;             /* number of 1 neighbours,      */
                                         /* k = total neighbours counted */

        /* horizontal neighbours */
        if (j > 0)    { ++k; if ((curr >> (j - 1)) & 1) ++ones; }
        if (j < 3)    { ++k; if ((curr >> (j + 1)) & 1) ++ones; }

        /* vertical neighbours */
        if (hasPrev) { ++k; if ((prev >> j) & 1) ++ones; }
        if (hasNext) { ++k; if ((next >> j) & 1) ++ones; }

        /* anti‑diagonal neighbours from the previous row (up‑right) */
        if (hasPrev) {
            if (j < 3) { ++k; if ((prev >> (j + 1)) & 1) ++ones; }
        }
        /* anti‑diagonal neighbours from the next row (down‑left) */
        if (hasNext) {
            if (j > 0) { ++k; if ((next >> (j - 1)) & 1) ++ones; }
        }

        if (ones > k / 2)
            return false;                /* 0 would be less than majority of 1s */
    }
    return true;
}

/* Build the static validity tables.  The tables are computed once. */
static void init_tables(void)
{
    if (tables_ready)
        return;

    for (int a = 0; a < 16; ++a)
    {
        valid_single[a] = row_valid_single(a);
        for (int b = 0; b < 16; ++b)
        {
            /* first row: no previous neighbour, next row present */
            valid_first[a][b]   = row_valid(0, a, b, false, true);
            /* last row: previous neighbour present, no next */
            valid_last[a][b]    = row_valid(a, b, 0, true, false);
            for (int c = 0; c < 16; ++c)
                valid_internal[a][b][c] = row_valid(a, b, c, true, true);
        }
    }
    tables_ready = true;
}

/* Sequence entry point: a(idx_t n) must handle all n >= 1 in arbitrary order.
 * For n == 1 we count single rows using valid_single.  For n >= 2 we use
 * the dynamic programme described above. */
seq_t a(idx_t n)
{
    init_tables();

    if (n < 1)
        return (seq_t)0;                 /* not needed for the official tests */

    if (n == 1) {
        seq_t total = (seq_t)0;
        for (int r = 0; r < 16; ++r)
            if (valid_single[r])
                ++total;
        return total;
    }

    /* dp[p][c] represented as dp[p*16 + c] */
    seq_t dp[256]      = {0};
    seq_t nextdp[256]  = {0};

    /* initialise with all pairs producing a valid first row */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            if (valid_first[a][b])
                dp[a * 16 + b] = (seq_t)1;

    /* Process rows 3 .. n.  For each new row c we verify row b now. */
    for (idx_t row = 2; row < n; ++row) {   /* row counts from 0‑based 0,1,... */
        for (int i = 0; i < 256; ++i)
            nextdp[i] = (seq_t)0;

        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                seq_t cur = dp[a * 16 + b];
                if (cur == (seq_t)0)
                    continue;
                for (int c = 0; c < 16; ++c) {
                    if (valid_internal[a][b][c])
                        nextdp[b * 16 + c] += cur;
                }
            }
        }
        for (int i = 0; i < 256; ++i)
            dp[i] = nextdp[i];
    }

    /* Add a final row: check last row validity */
    seq_t result = (seq_t)0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            if (dp[a * 16 + b] != (seq_t)0 && valid_last[a][b])
                result += dp[a * 16 + b];

    return result;
}
