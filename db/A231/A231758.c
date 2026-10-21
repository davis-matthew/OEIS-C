/*
 * A231758  Number of (n+1)X(2+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming by rows for A231758.
   The algorithm counts 3‑column arrays of height n+1 where no
   element has a strict majority of ones among its horizontal,
   diagonal (above), and antidiagonal (below) neighbours. */

#include <stdbool.h>

#define R 8   /* 2^3 possible row patterns */

/* Test whether a middle row `curr` satisfies the problem
   condition when the row above is `prev` (exists if hasPrev)
   and the row below is `next` (exists if hasNext). */
static inline bool
row_ok(int prev, int curr, int next, bool hasPrev, bool hasNext)
{
    for (int j = 0; j < 3; ++j) {
        int ones  = 0;
        int total = 0;

        /* horizontal neighbours */
        if (j > 0) { ++total; ones += (curr >> (j - 1)) & 1; }
        if (j < 2) { ++total; ones += (curr >> (j + 1)) & 1; }

        /* diagonal neighbours from the row above */
        if (hasPrev) {
            if (j > 0) { ++total; ones += (prev >> (j - 1)) & 1; }
            if (j < 2) { ++total; ones += (prev >> (j + 1)) & 1; }
        }

        /* antidiagonal neighbours from the row below */
        if (hasNext) {
            if (j > 0) { ++total; ones += (next >> (j - 1)) & 1; }
            if (j < 2) { ++total; ones += (next >> (j + 1)) & 1; }
        }

        if (ones > total / 2) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    seq_t dp[R][R];
    seq_t nxt[R][R];

    /* initialise dp for the first two rows */
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            dp[i][j] = 0;

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < R; ++c)
            if (row_ok(0, r, c, false, true))
                dp[r][c] = 1;

    /* special case: only two rows (n == 1) */
    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < R; ++c)
                if (dp[r][c] && row_ok(r, c, 0, true, false))
                    res += dp[r][c];
        return res;
    }

    /* add rows 2 .. n (total height n+1) */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next table */
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j)
                nxt[i][j] = 0;

        /* transition */
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < R; ++c) {
                seq_t val = dp[r][c];
                if (!val) continue;
                for (int nr = 0; nr < R; ++nr)
                    if (row_ok(r, c, nr, true, true))
                        nxt[c][nr] += val;
            }

        /* copy next to dp for next iteration */
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* final constraint for the bottom row */
    seq_t result = 0;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < R; ++c)
            if (dp[r][c] && row_ok(r, c, 0, true, false))
                result += dp[r][c];

    return result;
}
