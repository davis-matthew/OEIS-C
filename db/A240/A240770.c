/*
 * A240770  Number of nX2 0..3 arrays with no element equal to exactly one
 *          horizontal or vertical neighbor, with new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A240770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 208 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240770
   Number of n×2 arrays with values 0..3, filled row‑major,
   such that no element equals exactly one of its horizontal or
   vertical neighbors, and new values are introduced in order
   0,1,2,3.
   The recurrence was derived from a DP over the two most recent
   rows and the set of values used so far. */

#include <stdint.h>
#include <stdbool.h>

/* check constraints for the last row after all rows processed
   (bottom neighbor missing).  prevPrev = 16 -> top missing. */
static inline bool check_last_row(int prevPrev, int last)
{
    int leftLast  = last / 4;
    int rightLast = last % 4;
    int leftTop   = (prevPrev == 16) ? -1 : prevPrev / 4;
    int rightTop  = (prevPrev == 16) ? -1 : prevPrev % 4;

    /* left cell */
    int cntL = 0;
    if (leftLast == rightLast) ++cntL;
    if (prevPrev != 16 && leftLast == leftTop) ++cntL;
    if (cntL == 1) return false;

    /* right cell */
    int cntR = 0;
    if (leftLast == rightLast) ++cntR;
    if (prevPrev != 16 && rightLast == rightTop) ++cntR;
    if (cntR == 1) return false;

    return true;
}

/* check constraints for the 'prev' row when the next row 'next' is added.
   prevPrev = 16 means the row above is missing (top neighbor absent). */
static inline bool check_prev_row(int prevPrev, int prev, int next)
{
    /* if there is no previous row (prev==16) nothing to check */
    if (prev == 16)
        return true;

    int leftPrev  = prev / 4;
    int rightPrev = prev % 4;
    int leftNext  = next / 4;
    int rightNext = next % 4;
    int leftTop   = (prevPrev == 16) ? -1 : prevPrev / 4;
    int rightTop  = (prevPrev == 16) ? -1 : prevPrev % 4;

    /* left cell of prev */
    int cntL = 0;
    if (leftPrev == rightPrev) ++cntL;          /* horizontal neighbor */
    if (prevPrev != 16 && leftPrev == leftTop) ++cntL;   /* top neighbor */
    if (leftNext == leftPrev) ++cntL;          /* bottom neighbor */
    if (cntL == 1) return false;

    /* right cell of prev */
    int cntR = 0;
    if (leftPrev == rightPrev) ++cntR;
    if (prevPrev != 16 && rightPrev == rightTop) ++cntR;
    if (rightNext == rightPrev) ++cntR;
    if (cntR == 1) return false;

    return true;
}

/* helper: smallest unused value in 0..3 */
static inline unsigned char smallest_missing(unsigned char mask)
{
    for (int v = 0; v < 4; ++v)
        if (!(mask & (1U << v)))
            return (unsigned char)v;
    return 4;   /* all used */
}

/* pre‑computed tables */
static bool  feasible[16][16];
static unsigned char newMaskArr[16][16];

/* build feasibility and mask transition tables for a new row */
static void precompute_feasible(void)
{
    for (int mask = 0; mask < 16; ++mask)
        for (int code = 0; code < 16; ++code) {
            unsigned char cur = (unsigned char)mask;
            bool ok = true;
            /* two cells left/right */
            int v0 = code / 4;
            int v1 = code % 4;
            for (int pos = 0; pos < 2 && ok; ++pos) {
                int v = (pos == 0) ? v0 : v1;
                if (cur & (1U << v))
                    continue;           /* already used */
                unsigned char need = smallest_missing(cur);
                if (v != need) {
                    ok = false;
                    break;
                }
                cur |= (1U << v);        /* book‑keep usage */
            }
            feasible[mask][code] = ok;
            newMaskArr[mask][code] = cur;
        }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the sequence */

    /* one‑time precomputation */
    static bool precomputed = false;
    if (!precomputed) {
        precompute_feasible();
        precomputed = true;
    }

    /* DP arrays: dp[mask][prevPrev][prev] */
    static seq_t dp[16][17][17];
    static seq_t next_dp[16][17][17];
    for (int m = 0; m < 16; ++m)
        for (int pp = 0; pp < 17; ++pp)
            for (int p = 0; p < 17; ++p)
                dp[m][pp][p] = 0;

    dp[0][16][16] = 1;   /* initial empty state */

    for (idx_t row = 0; row < n; ++row) {
        /* zero next_dp */
        for (int m = 0; m < 16; ++m)
            for (int pp = 0; pp < 17; ++pp)
                for (int p = 0; p < 17; ++p)
                    next_dp[m][pp][p] = 0;

        /* transition */
        for (int mask = 0; mask < 16; ++mask)
            for (int prevPrev = 0; prevPrev < 17; ++prevPrev)
                for (int prev = 0; prev < 17; ++prev) {
                    seq_t cnt = dp[mask][prevPrev][prev];
                    if (cnt == 0) continue;

                    for (int curr = 0; curr < 16; ++curr) {
                        if (!feasible[mask][curr]) continue;
                        if (!check_prev_row(prevPrev, prev, curr)) continue;

                        unsigned char newMask = newMaskArr[mask][curr];
                        next_dp[newMask][prev][curr] += cnt;
                    }
                }

        /* copy next_dp to dp for next iteration */
        for (int m = 0; m < 16; ++m)
            for (int pp = 0; pp < 17; ++pp)
                for (int p = 0; p < 17; ++p)
                    dp[m][pp][p] = next_dp[m][pp][p];
    }

    /* final state: check last row constraints */
    seq_t result = 0;
    for (int mask = 0; mask < 16; ++mask)
        for (int prevPrev = 0; prevPrev < 17; ++prevPrev)
            for (int prev = 0; prev < 17; ++prev) {
                seq_t cnt = dp[mask][prevPrev][prev];
                if (cnt == 0) continue;
                if (!check_last_row(prevPrev, prev)) continue;
                result += cnt;
            }
    return result;
}
