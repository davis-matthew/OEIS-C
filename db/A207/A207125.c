/*
 * A207125  Number of 5 X n 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 5 × n binary arrays avoiding
 * 0 0 1 and 0 1 0 horizontally and 0 0 1 and 0 1 1 vertically.
 * This is done by a DP over the last two columns.
 * States: (first,last) pair of allowed columns.
 * Transition: add a new column if it together with the previous two
 * columns does not create a forbidden horizontal pattern in any row.
 * The DP counts all sequences of length n.  Complexity is
 * O(n · states²) with states = 12² = 144, negligible for n≤10⁴.  */

#include <stdint.h>
#include <stdbool.h>

/* Return the a(n) value for n ≥ 1 */
seq_t a(idx_t n)
{
    /* First‑time construction of tables */
    static bool done = false;
    /* Allowed columns (5‑bit masks) */
    static int allowed[32];
    static int allowedCnt;
    /* Transition tables : for each state (i,j) store list of destinations */
    static int transCnt[144];
    static int transDest[144][12];
    /* DP arrays (size 144) */
    static seq_t dp[144];
    static seq_t ndp[144];

    if (!done) {
        /* Enumerate all 32 potential columns and keep those that satisfy the
         * vertical constraints: no 0?1 patterns with distance 2.
         */
        allowedCnt = 0;
        for (int m = 0; m < 32; ++m) {
            bool ok = true;
            int b0 = (m >> 0) & 1;
            int b1 = (m >> 1) & 1;
            int b2 = (m >> 2) & 1;
            int b3 = (m >> 3) & 1;
            int b4 = (m >> 4) & 1;
            if (b0 == 0 && b2 == 1) ok = false;
            if (b1 == 0 && b3 == 1) ok = false;
            if (b2 == 0 && b4 == 1) ok = false;
            if (ok) {
                allowed[allowedCnt++] = m;
            }
        }

        /* Precompute transitions: for each state (i,j) and each new column k
         * check horizontal constraints.  The state index is i*allowedCnt + j.
         */
        for (int i = 0; i < allowedCnt; ++i) {
            for (int j = 0; j < allowedCnt; ++j) {
                int stateIdx = i * allowedCnt + j;
                transCnt[stateIdx] = 0;
                int mi = allowed[i];
                int mj = allowed[j];
                for (int k = 0; k < allowedCnt; ++k) {
                    int mk = allowed[k];
                    bool ok = true;
                    for (int r = 0; r < 5; ++r) {
                        int aBit = (mi >> r) & 1;
                        int bBit = (mj >> r) & 1;
                        int cBit = (mk >> r) & 1;
                        int triple = (aBit << 2) | (bBit << 1) | cBit;
                        if (triple == 1 || triple == 2) { /* 001 or 010 */
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        int destIdx = j * allowedCnt + k;
                        transDest[stateIdx][transCnt[stateIdx]++] = destIdx;
                    }
                }
            }
        }

        done = true;
    }

    /* Base cases */
    if (n == 0) return 0;          /* undefined but harmless */
    if (n == 1) return (seq_t)allowedCnt;
    if (n == 2) return (seq_t)allowedCnt * (seq_t)allowedCnt;

    /* DP over columns, starting with all 2‑column sequences */
    int states = allowedCnt * allowedCnt;
    for (int s = 0; s < states; ++s) dp[s] = 1;  /* each (i,j) pair counts once */

    /* Step through columns 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < states; ++s) ndp[s] = 0;
        for (int s = 0; s < states; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            int tCnt = transCnt[s];
            for (int t = 0; t < tCnt; ++t) {
                int d = transDest[s][t];
                ndp[d] += cur;
            }
        }
        for (int s = 0; s < states; ++s) dp[s] = ndp[s];
    }

    /* Sum over ending states */
    seq_t ans = 0;
    for (int s = 0; s < states; ++s) ans += dp[s];
    return ans;
}
