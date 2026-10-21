/*
 * A231704  Number of (n+1)X(2+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231704: number of (n+1)×3 0..1 arrays with no element having a strict majority
   of its horizontal, vertical and antidiagonal neighbors equal to one. */
#include <stdbool.h>

/* Test whether a row satisfies its neighbor constraint when the rows above and below
   are given as 'up' and 'down'.  If a neighbor does not exist, its code is <0. */
static bool row_ok(unsigned row, int up, int down)
{
    for (int j = 0; j < 3; ++j) {
        unsigned count = 0;
        unsigned ones  = 0;

        /* left */
        if (j > 0) {
            count++;
            if ((row >> (j - 1)) & 1U) ones++;
        }
        /* right */
        if (j < 2) {
            count++;
            if ((row >> (j + 1)) & 1U) ones++;
        }
        /* up */
        if (up >= 0) {
            count++;
            if ((up >> j) & 1U) ones++;
        }
        /* down */
        if (down >= 0) {
            count++;
            if ((down >> j) & 1U) ones++;
        }
        /* up‑right */
        if (up >= 0 && j < 2) {
            count++;
            if ((up >> (j + 1)) & 1U) ones++;
        }
        /* down‑left */
        if (down >= 0 && j > 0) {
            count++;
            if ((down >> (j - 1)) & 1U) ones++;
        }

        if (ones * 2 > count) return false;  /* strict majority */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return 0;            /* offset 1 – unused case */

    idx_t rows = n + 1ULL;              /* total how many rows in the array */

    seq_t dp[64], next[64];
    for (int i = 0; i < 64; ++i) dp[i] = 0;

    /* initial two rows – first row must satisfy its constraint with the second row */
    for (unsigned r0 = 0; r0 < 8U; ++r0) {
        for (unsigned r1 = 0; r1 < 8U; ++r1) {
            if (row_ok(r0, -1, r1))
                dp[(r0 << 3) | r1] = 1;
        }
    }

    /* add remaining rows; when a new row is chosen, the middle row's constraint is checked */
    for (idx_t step = 2; step < rows; ++step) {
        for (int i = 0; i < 64; ++i) next[i] = 0;
        for (unsigned prevPrev = 0; prevPrev < 8U; ++prevPrev) {
            for (unsigned prev = 0; prev < 8U; ++prev) {
                seq_t cur = dp[(prevPrev << 3) | prev];
                if (cur == 0) continue;
                for (unsigned curRow = 0; curRow < 8U; ++curRow) {
                    if (!row_ok(prev, prevPrev, curRow)) continue;
                    next[(prev << 3) | curRow] += cur;
                }
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = next[i];
    }

    /* final row must satisfy its constraint with the previous row */
    seq_t result = 0;
    for (unsigned prevPrev = 0; prevPrev < 8U; ++prevPrev) {
        for (unsigned last = 0; last < 8U; ++last) {
            if (row_ok(last, prevPrev, -1))
                result += dp[(prevPrev << 3) | last];
        }
    }
    return result;
}
