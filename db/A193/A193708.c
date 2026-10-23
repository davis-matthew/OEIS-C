/*
 * A193708  Number of arrays of -6..6 integers x(1..n) with every x(i) in a
 *          subsequence of length 1, 2, 3 or 4 with sum zero.
 * https://oeis.org/A193708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays x(1..n) with x(i) in {-6,…,6} such that
 *            every element lies in some contiguous subsequence of length
 *            1,2,3 or 4 whose sum is zero. Computation is performed by a DP
 *            automaton that keeps the last three values and their coverage
 *            status
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */

seq_t a(idx_t n)
{
    /*--- State encoding ----------------------------------------------------*/
    /* Each state encodes the last three values (a,b,c) and a 3‑bit mask
       of their coverage.  Values are in [-6,6] -> 13 possibilities.
       Total states = 8 (mask) * 13^3 = 17576.                     */
    enum { VALS = 13, VALS3 = VALS*VALS*VALS, STATES = 8*VALS3, K = 13 };

    /* static tables that are built only once */
    static int *a_val = NULL;
    static int *b_val = NULL;
    static int *c_val = NULL;
    static unsigned char *mask_val = NULL;
    static int *trans0 = NULL;     /* transition table for i>=4 */

    /* Build tables on first call */
    if (!a_val) {
        a_val = (int*)malloc(STATES * sizeof(int));
        b_val = (int*)malloc(STATES * sizeof(int));
        c_val = (int*)malloc(STATES * sizeof(int));
        mask_val = (unsigned char*)malloc(STATES * sizeof(unsigned char));
        /* decode all states */
        for (int mask = 0; mask < 8; ++mask) {
            for (int idx = 0; idx < VALS3; ++idx) {
                int state = mask * VALS3 + idx;
                int a = (idx / (VALS*VALS)) - 6;
                int r = idx % (VALS*VALS);
                int b = (r / VALS) - 6;
                int c = (r % VALS) - 6;
                a_val[state] = a;
                b_val[state] = b;
                c_val[state] = c;
                mask_val[state] = (unsigned char)mask;
            }
        }
        /* transition table for i >= 4 (all window lengths allowed) */
        trans0 = (int*)malloc(STATES * K * sizeof(int));
        for (int s = 0; s < STATES; ++s) {
            int a = a_val[s];
            int b = b_val[s];
            int c = c_val[s];
            int mask = mask_val[s];
            for (int dIdx = 0; dIdx < K; ++dIdx) {
                int d = dIdx - 6;
                /* sums for all window lengths */
                int z1 = (d == 0);
                int z2 = (c + d == 0);
                int z3 = (b + c + d == 0);
                int z4 = (a + b + c + d == 0);
                int nc0 = ((mask >> 0) & 1) | z4;          /* i-3 */
                int nc1 = ((mask >> 1) & 1) | z3 | z4;      /* i-2 */
                int nc2 = ((mask >> 2) & 1) | z2 | z3 | z4; /* i-1 */
                int nc3 = z1 | z2 | z3 | z4;                /* i   */
                if (!nc0) {
                    trans0[s * K + dIdx] = -1;              /* invalid */
                    continue;
                }
                int newA = b;
                int newB = c;
                int newC = d;
                int newMask = (nc1 << 0) | (nc2 << 1) | (nc3 << 2);
                int newIdx = newMask * VALS3 +
                             ((newA + 6) * VALS * VALS) +
                             ((newB + 6) * VALS) +
                             (newC + 6);
                trans0[s * K + dIdx] = newIdx;
            }
        }
    }

    /* DP arrays */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    if (!dp_cur) {
        dp_cur = (seq_t*)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t*)malloc(STATES * sizeof(seq_t));
    }
    /* zero dp_cur */
    memset(dp_cur, 0, STATES * sizeof(seq_t));

    /* start state: all dummy positions considered already covered (mask=7),
       values 0 0 0 (encoded in indices). */
    int startMask = 7;
    int startValsIdx = ((0 + 6) * VALS * VALS) + ((0 + 6) * VALS) + (0 + 6);
    int startState = startMask * VALS3 + startValsIdx;
    dp_cur[startState] = 1;

    /* Main loop over positions */
    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next DP */
        memset(dp_next, 0, STATES * sizeof(seq_t));

        if (pos == 1) { /* only length 1 windows possible */
            for (int s = 0; s < STATES; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                int a = a_val[s], b = b_val[s], c = c_val[s];
                int mask = mask_val[s];
                for (int dIdx = 0; dIdx < K; ++dIdx) {
                    int d = dIdx - 6;
                    int z1 = (d == 0);
                    /* cov for i-3,i-2,i-1 unchanged */
                    int nc0 = (mask >> 0) & 1;
                    int nc1 = (mask >> 1) & 1;
                    int nc2 = (mask >> 2) & 1;
                    int nc3 = z1;                           /* i   */
                    int newMask = (nc1 << 0) | (nc2 << 1) | (nc3 << 2);
                    int newIdx = newMask * VALS3 +
                                 ((b + 6) * VALS * VALS) +
                                 ((c + 6) * VALS) +
                                 (d + 6);
                    dp_next[newIdx] += val;
                }
            }
        } else if (pos == 2) { /* lengths 1 and 2 */
            for (int s = 0; s < STATES; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                int a = a_val[s], b = b_val[s], c = c_val[s];
                int mask = mask_val[s];
                for (int dIdx = 0; dIdx < K; ++dIdx) {
                    int d = dIdx - 6;
                    int z1 = (d == 0);
                    int z2 = (c + d == 0);
                    int z3 = 0, z4 = 0;
                    int nc0 = (mask >> 0) & 1;                /* i-3 already djn */
                    int nc1 = (mask >> 1) & 1;
                    int nc2 = (mask >> 2) & 1 | z2;
                    int nc3 = z1 | z2;
                    if (!nc0) continue;
                    int newMask = (nc1 << 0) | (nc2 << 1) | (nc3 << 2);
                    int newIdx = newMask * VALS3 +
                                 ((b + 6) * VALS * VALS) +
                                 ((c + 6) * VALS) +
                                 (d + 6);
                    dp_next[newIdx] += val;
                }
            }
        } else if (pos == 3) { /* lengths 1,2,3 */
            for (int s = 0; s < STATES; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                int a = a_val[s], b = b_val[s], c = c_val[s];
                int mask = mask_val[s];
                for (int dIdx = 0; dIdx < K; ++dIdx) {
                    int d = dIdx - 6;
                    int z1 = (d == 0);
                    int z2 = (c + d == 0);
                    int z3 = (b + c + d == 0);
                    int z4 = 0;
                    int nc0 = (mask >> 0) & 1;                /* i-3 already covered */
                    int nc1 = (mask >> 1) & 1 | z3;
                    int nc2 = (mask >> 2) & 1 | z2 | z3;
                    int nc3 = z1 | z2 | z3;
                    if (!nc0) continue;
                    int newMask = (nc1 << 0) | (nc2 << 1) | (nc3 << 2);
                    int newIdx = newMask * VALS3 +
                                 ((b + 6) * VALS * VALS) +
                                 ((c + 6) * VALS) +
                                 (d + 6);
                    dp_next[newIdx] += val;
                }
            }
        } else { /* pos >= 4, all lengths allowed; use precomputed table */
            for (int s = 0; s < STATES; ++s) {
                seq_t val = dp_cur[s];
                if (!val) continue;
                for (int dIdx = 0; dIdx < K; ++dIdx) {
                    int ns = trans0[s * K + dIdx];
                    if (ns < 0) continue;
                    dp_next[ns] += val;
                }
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over states where all three last positions are covered */
    seq_t res = 0;
    for (int s = 0; s < STATES; ++s) {
        if (mask_val[s] == 7) {        /* all bits set */
            res += dp_cur[s];
        }
    }
    return res;
}
