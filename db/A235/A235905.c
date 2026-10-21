/*
 * A235905  Number of (n+1) X (2+1) 0..2 arrays with the minimum plus the
 *          maximum unequal to the lower median plus the upper median in every
 *          2 X 2 subblock.
 * https://oeis.org/A235905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×3 arrays with entries 0‑2 such that
 *            for every 2×2 subblock the minimum plus the maximum is NOT equal
 *            to the lower median plus the upper median. This is implemented
 *            by a DP over the 27 possible rows of 3 cells
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   state_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/state_digits.h"

static bool _trans_ok[27][27];
static bool _trans_init = false;

/* sort four integers a,b,c,d and return true if min+max != median1+median2 */
static bool block_ok(int a, int b, int c, int d) {
    int v[4] = {a, b, c, d};
    /* sorting network for 4 elements */
    if (v[0] > v[1]) { int t=v[0];v[0]=v[1];v[1]=t; }
    if (v[2] > v[3]) { int t=v[2];v[2]=v[3];v[3]=t; }
    if (v[0] > v[2]) { int t=v[0];v[0]=v[2];v[2]=t; }
    if (v[1] > v[3]) { int t=v[1];v[1]=v[3];v[3]=t; }
    if (v[1] > v[2]) { int t=v[1];v[1]=v[2];v[2]=t; }
    return (v[0] + v[3]) != (v[1] + v[2]);
}

/* precompute allowed transitions */
static void init_transitions(void) {
    if (_trans_init) return;
    for (idx_t s1 = 0; s1 < 27; ++s1) {
        int a[3];
        state_digits(s1, a);
        for (idx_t s2 = 0; s2 < 27; ++s2) {
            int b[3];
            state_digits(s2, b);
            if (block_ok(a[0], a[1], b[0], b[1]) &&
                block_ok(a[1], a[2], b[1], b[2]))
                _trans_ok[s1][s2] = true;
            else
                _trans_ok[s1][s2] = false;
        }
    }
    _trans_init = true;
}

/* main function as required by the template */
seq_t a(idx_t n)
{
    /* n is at least 1.  The array height is n+1 rows. */
    init_transitions();

    /* DP arrays for current row count. */
    seq_t dp[27];
    seq_t ndp[27];

    /* initial row can be anything */
    for (idx_t i = 0; i < 27; ++i) dp[i] = (seq_t)1;

    idx_t rows = n + 1;          /* total rows */
    for (idx_t r = 2; r <= rows; ++r) {   /* r is current row number */
        for (idx_t i = 0; i < 27; ++i) ndp[i] = (seq_t)0;
        for (idx_t s1 = 0; s1 < 27; ++s1) {
            if (dp[s1] == 0) continue;
            for (idx_t s2 = 0; s2 < 27; ++s2) {
                if (_trans_ok[s1][s2]) {
                    ndp[s2] += dp[s1];
                }
            }
        }
        /* swap arrays */
        for (idx_t i = 0; i < 27; ++i) dp[i] = ndp[i];
    }

    /* sum all possible ending rows */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 27; ++i) total += dp[i];
    return total;
}
