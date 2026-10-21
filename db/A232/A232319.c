/*
 * A232319  Number of (3+1) X (n+1) 0..1 arrays with every element equal to
 *          some horizontal or antidiagonal neighbor, with top left element
 *          zero.
 * https://oeis.org/A232319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A232319 (Number of (3+1)×(n+1) 0..1 arrays with every element
    equal to some horizontal or anti‑diagonal neighbor, top‑left 0)
    This implementation uses dynamic programming over columns.
    For each column we maintain the pattern of the last two columns;
    the transition checks that the middle column satisfies the
    neighbor rule using its left (prev) and right (next) columns.
    The algorithm runs in O(n) time with only 16×16 states. */

#include <stdint.h>

/* Constants for the 4 rows (bits 0..3 correspond to rows 0..3). */
#define ROWS 4
#define STATES (1 << ROWS)

/* Return the number a(n) for n >= 1.  a(0) is undefined (returns 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* dp[prev][curr] – number of ways up to current column with
       previous column pattern = prev and current column pattern = curr. */
    seq_t dp[STATES][STATES] = { { 0 } };
    seq_t nextdp[STATES][STATES] = { { 0 } };

    /* ---------- Initial column 0 and 1 ---------------------------------- */
    /* Top‑left element must be 0: bit 0 of column 0 is 0. */
    for (int c0 = 0; c0 < STATES; ++c0) {
        if ((c0 & 1) != 0)   /* row 0 bit must be 0 */
            continue;
        for (int c1 = 0; c1 < STATES; ++c1) {
            /* Check column 0 with its only neighbor (column 1). */
            int ok = 1;
            for (int r = 0; r < ROWS; ++r) {
                int cur = (c0 >> r) & 1;
                int good = 0;
                /* neighbor to the right in same row */
                if (((c1 >> r) & 1) == cur)
                    good = 1;
                /* neighbor down–right */
                if (!good && r < ROWS - 1 && (((c1 >> (r + 1)) & 1) == cur))
                    good = 1;
                if (!good) {
                    ok = 0;
                    break;
                }
            }
            if (ok)
                dp[c0][c1] = dp[c0][c1] + (seq_t)1;
        }
    }

    /* ---------- Special case n == 1 ------------------------------------- */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int c0 = 0; c0 < STATES; ++c0)
            for (int c1 = 0; c1 < STATES; ++c1) {
                if (dp[c0][c1] == 0)
                    continue;
                /* Column 1 now has no right neighbour. */
                int ok = 1;
                for (int r = 0; r < ROWS; ++r) {
                    int cur = (c1 >> r) & 1;
                    int good = 0;
                    /* left same row */
                    if (((c0 >> r) & 1) == cur)
                        good = 1;
                    /* left up‑left */
                    if (!good && r > 0 && (((c0 >> (r - 1)) & 1) == cur))
                        good = 1;
                    if (!good) {
                        ok = 0;
                        break;
                    }
                }
                if (ok)
                    ans = ans + dp[c0][c1];
            }
        return ans;
    }

    /* ---------- DP for columns 2 … n ------------------------------------ */
    for (idx_t step = 2; step <= n; ++step) {
        /* reset nextdp */
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                nextdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == (seq_t)0)
                    continue;
                for (int nxt = 0; nxt < STATES; ++nxt) {
                    /* Validate column 'curr' using 'prev' and 'nxt'. */
                    int ok = 1;
                    for (int r = 0; r < ROWS; ++r) {
                        int cur = (curr >> r) & 1;
                        int good = 0;
                        /* left same row */
                        if (((prev >> r) & 1) == cur)
                            good = 1;
                        /* left up‑left */
                        if (!good && r > 0 && (((prev >> (r - 1)) & 1) == cur))
                            good = 1;
                        /* right same row */
                        if (!good && ((nxt >> r) & 1) == cur)
                            good = 1;
                        /* right down‑right */
                        if (!good && r < ROWS - 1 && (((nxt >> (r + 1)) & 1) == cur))
                            good = 1;
                        if (!good) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok)
                        nextdp[curr][nxt] = nextdp[curr][nxt] + ways;
                }
            }
        }
        /* copy nextdp to dp for next iteration */
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* ---------- Final column (n) constraints ----------------------------- */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways == (seq_t)0)
                continue;
            int ok = 1;
            for (int r = 0; r < ROWS; ++r) {
                int cur = (curr >> r) & 1;
                int good = 0;
                /* left same row */
                if (((prev >> r) & 1) == cur)
                    good = 1;
                /* left up‑left */
                if (!good && r > 0 && (((prev >> (r - 1)) & 1) == cur))
                    good = 1;
                if (!good) {
                    ok = 0;
                    break;
                }
            }
            if (ok)
                ans = ans + ways;
        }
    }

    return ans;
}
