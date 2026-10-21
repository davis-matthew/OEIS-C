/*
 * A231802  Number of (n+1)X(4+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to one.
 * https://oeis.org/A231802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A231802
 *  a(n) = number of (n+1)×5 binary arrays with no element having a strict majority
 *         of its horizontal, vertical, diagonal and antidiagonal neighbours equal to one.
 *  The implementation uses a row‑by‑row DP over the 5 columns (32 possible row masks).
 *  First and last rows are handled with special boundary checks.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define STATES 32          /* 2^5 possible rows */
#define COLS   5

/* ---------- helpers ---------- */

static inline bool cell_ok(int rPrev, int rCur, int rNext,
                           int col, bool hasPrev, bool hasNext)
{
    int cnt = 0;

    /* horizontal neighbours */
    if (col > 0)   cnt += (rCur >> (col - 1)) & 1;
    if (col < COLS-1) cnt += (rCur >> (col + 1)) & 1;

    /* vertical neighbours */
    if (hasPrev)   cnt += (rPrev >> col) & 1;
    if (hasNext)   cnt += (rNext >> col) & 1;

    /* diagonal neighbours */
    if (hasPrev) {
        if (col > 0)   cnt += (rPrev >> (col - 1)) & 1;
        if (col < COLS-1) cnt += (rPrev >> (col + 1)) & 1;
    }
    if (hasNext) {
        if (col > 0)   cnt += (rNext >> (col - 1)) & 1;
        if (col < COLS-1) cnt += (rNext >> (col + 1)) & 1;
    }

    /* degree of the cell (number of possible neighbours) */
    int deg =
        (col > 0) + (col < COLS-1)               /* horizontal */
        + (hasPrev) + (hasNext)                 /* vertical */
        + (hasPrev && (col > 0)) + (hasPrev && (col < COLS-1))
        + (hasNext && (col > 0)) + (hasNext && (col < COLS-1));

    /* no strict majority of ones */
    return cnt <= deg / 2;
}

static inline bool row_ok(int rPrev, int rCur, int rNext,
                          bool hasPrev, bool hasNext)
{
    for (int col = 0; col < COLS; ++col) {
        if (!cell_ok(rPrev, rCur, rNext, col, hasPrev, hasNext))
            return false;
    }
    return true;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    /* transition tables, built once */
    static bool validTop[STATES][STATES];
    static bool validBottom[STATES][STATES];
    static uint32_t nextMask[STATES][STATES];
    static bool table_ready = false;

    if (!table_ready) {
        for (int prev = 0; prev < STATES; ++prev) {
            for (int cur = 0; cur < STATES; ++cur) {
                uint32_t mask = 0;
                for (int next = 0; next < STATES; ++next) {
                    if (row_ok(prev, cur, next, true, true))
                        mask |= (1u << next);
                }
                nextMask[prev][cur] = mask;
                validTop[prev][cur]     = row_ok(0, prev, cur, false, true);
                validBottom[prev][cur]  = row_ok(prev, cur, 0, true, false);
            }
        }
        table_ready = true;
    }

    /* number of rows in the array = n + 1 */
    idx_t numRows = n + (idx_t)1;
    if (numRows < 2) return (seq_t)0;   /* safety for n==0 */

    /* DP tables */
    static seq_t dp1[STATES][STATES];
    static seq_t dp2[STATES][STATES];
    seq_t (*curdp)[STATES] = dp1;
    seq_t (*nextdp)[STATES] = dp2;

    /* reset DP tables */
    for (int i = 0; i < STATES; ++i)
        for (int j = 0; j < STATES; ++j) {
            curdp[i][j] = (seq_t)0;
            nextdp[i][j] = (seq_t)0;
        }

    /* initial two rows (top boundary) */
    for (int r0 = 0; r0 < STATES; ++r0)
        for (int r1 = 0; r1 < STATES; ++r1)
            if (validTop[r0][r1])
                curdp[r0][r1] = (seq_t)1;

    /* process remaining rows */
    for (idx_t step = 2; step < numRows; ++step) {
        /* zero nextdp */
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                nextdp[i][j] = (seq_t)0;

        /* transition */
        for (int prev = 0; prev < STATES; ++prev) {
            for (int cur = 0; cur < STATES; ++cur) {
                seq_t ways = curdp[prev][cur];
                if (ways == (seq_t)0) continue;
                uint32_t mask = nextMask[prev][cur];
                while (mask) {
                    int nxt = __builtin_ctz(mask);
                    mask &= mask - 1;
                    nextdp[cur][nxt] += ways;
                }
            }
        }

        /* swap tables */
        seq_t (*tmp)[STATES] = curdp;
        curdp = nextdp;
        nextdp = tmp;
    }

    /* apply bottom boundary and sum */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < STATES; ++prev)
        for (int cur = 0; cur < STATES; ++cur)
            if (validBottom[prev][cur])
                ans += curdp[prev][cur];

    return ans;
}
