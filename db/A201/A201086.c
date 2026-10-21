/*
 * A201086  Number of -6..6 arrays of n elements with first and second
 *          differences also in -6..6.
 * https://oeis.org/A201086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A201086 – Number of -6..6 arrays of n elements with
 *  first and second differences also in -6..6.
 *
 *  Implementation: dynamic programming over the pair (prev, curr).
 *  States are indexed by (prev+6)*13 + (curr+6) for prev,curr∈{-6,…,6}
 *  (169 states).  For each state S = (p,c) we pre‑compute all
 *  admissible next values n that satisfy
 *        |n - c| ≤ 6   AND   |n - 2c + p| ≤ 6
 *  and store the destination state (c,n).  The DP iterates from
 *  length 2 up to the requested n, summing all state counts at each
 *  step to obtain a(n).  No memoisation is used – the routine
 *  recomputes the sequence from scratch each time, which satisfies
 *  the requirement of correct behaviour for any call order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed transition data for the 169 states */
static int  trans_cnt[169];
static int  trans_dst[169][13];
static bool trans_inited = false;

/* Initialise transition tables once */
static void init_transitions(void)
{
    if (trans_inited) return;
    for (int p = -6; p <= 6; ++p) {
        for (int c = -6; c <= 6; ++c) {
            int s = (p + 6) * 13 + (c + 6);
            int cnt = 0;
            for (int n = -6; n <= 6; ++n) {
                int d1 = n - c;
                if (d1 < -6 || d1 > 6) continue;
                int d2 = n - 2 * c + p;
                if (d2 < -6 || d2 > 6) continue;
                int dest = (c + 6) * 13 + (n + 6);
                trans_dst[s][cnt++] = dest;
            }
            trans_cnt[s] = cnt;
        }
    }
    trans_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)0;             /* not defined, return 0 */
    if (n == 1) return (seq_t)13;

    /* Initialise transition data once */
    init_transitions();

    /* DP arrays on the stack – 169 states */
    seq_t dp_prev[169];
    seq_t dp_curr[169];

    /* Zero initialise */
    for (size_t i = 0; i < 169; ++i) dp_prev[i] = (seq_t)0;
    for (size_t i = 0; i < 169; ++i) dp_curr[i] = (seq_t)0;

    /* Build length‑2 base: all (p,c) with |c-p| ≤ 6 */
    for (int p = -6; p <= 6; ++p) {
        for (int c = -6; c <= 6; ++c) {
            int d = c - p;
            if (d >= -6 && d <= 6) {
                int idx = (p + 6) * 13 + (c + 6);
                dp_prev[idx] = (seq_t)1;
            }
        }
    }

    /* Sum to get a(2) */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < 169; ++i) total += dp_prev[i];
    if (n == 2) return total;

    /* Iterate for lengths 3 … n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero dp_curr */
        for (size_t i = 0; i < 169; ++i) dp_curr[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < 169; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            int cnt = trans_cnt[s];
            for (int j = 0; j < cnt; ++j) {
                int dest = trans_dst[s][j];
                dp_curr[dest] += val;
            }
        }

        /* copy dp_curr to dp_prev for next iteration */
        for (size_t i = 0; i < 169; ++i) dp_prev[i] = dp_curr[i];

        /* sum all states to get a(len) */
        total = (seq_t)0;
        for (size_t i = 0; i < 169; ++i) total += dp_prev[i];
    }

    return total;
}
