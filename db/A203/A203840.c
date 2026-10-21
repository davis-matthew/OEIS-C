/*
 * A203840  Number of n X 2 0..3 arrays with every nonzero element less than
 *          or equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   col_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A203840
    Count of n × 2 arrays with entries 0–3 such that each
    non‑zero element is ≤ at least two of its horizontal or
    vertical neighbours.  The algorithm uses dynamic programming
    over pairs of consecutive rows, exploiting that each row has
    only 16 possible patterns.  Caching is not required; the
    complexity is O(n·256) with constant factor < 10.
*/

#include <stdbool.h>
#include <stddef.h> /* for size_t */
#include "../utils/col_value.h"

#define DUMMY 16  /* index used for the nonexistent previous row */

/*
    Check whether the middle row `mid` satisfies the constraint
    given the row above `prev` (may be DUMMY for non‑existent) and
    the row below `next`  (may be -1 for missing below, e.g. bottom row).
*/
static inline bool row_ok(int prev, int mid, int next)
{
    for (int c = 0; c < 2; ++c) {
        int v = col_value(mid, c);
        if (v == 0) continue;                     /* no restriction on zeros */
        int cnt = 0;
        if (prev != DUMMY) {
            int a = col_value(prev, c);
            if (a >= v) cnt++;
        }
        if (next != -1) {
            int b = col_value(next, c);
            if (b >= v) cnt++;
        }
        int h = col_value(mid, 1 - c);             /* horizontal neighbor */
        if (h >= v) cnt++;
        if (cnt < 2) return false;                 /* fails the condition */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */

    /* DP arrays: dp[prev][mid], last row is mid,
                  prev row is prev (or DUMMY if none),  prev can be 0..15, DUMMY=16 */
    seq_t dp[17][16] = {0};
    seq_t ndp[17][16];
    const int max_pat = 16;          /* 0..15 valid patterns */

    /* Base: only one row, prev = DUMMY, mid = first row pattern */
    for (int mid = 0; mid < max_pat; ++mid) {
        dp[DUMMY][mid] = 1;
    }

    /* Process rows 2 … n */
    for (size_t row = 2; row <= (size_t)n; ++row) {
        /* clear ndp */
        for (int i = 0; i <= DUMMY; ++i)
            for (int j = 0; j < max_pat; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev <= DUMMY; ++prev)
            for (int mid = 0; mid < max_pat; ++mid) {
                seq_t ways = dp[prev][mid];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < max_pat; ++nxt) {
                    if (!row_ok(prev, mid, nxt)) continue;
                    ndp[mid][nxt] += ways;
                }
            }
        /* copy ndp to dp for next iteration */
        for (int i = 0; i <= DUMMY; ++i)
            for (int j = 0; j < max_pat; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final answer: sum over states where the last row satisfies its constraint
       (no row below, next = -1). */
    seq_t result = 0;
    for (int prev = 0; prev <= DUMMY; ++prev)
        for (int mid = 0; mid < max_pat; ++mid) {
            if (!row_ok(prev, mid, -1)) continue;
            result += dp[prev][mid];
        }

    return result;
}
