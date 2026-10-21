/*
 * A203722  Number of (n+1)X3 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 24, u64 n <= 68, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A203722
 *  Number of (n+1)×3 arrays with entries 0..2 such that for all
 *  2≤j≤3 and 2≤i≤n+1:
 *      a(i,j)+a(i,j-1) ≥ a(i-1,j)+a(i-1,j-1)   (column pair sums nondecreasing)
 *      a(i,j)+a(i-1,j) ≥ a(i,j-1)+a(i-1,j-1)   (row pair sums nondecreasing)
 *  The function returns this number for the given n (offset 1).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    enum { STATES = 27 };
    static unsigned char row[STATES][3];
    static unsigned char trans[STATES][STATES];
    static int precomp = 0;

    /* --- pre‑computation of row vectors and transition matrix --- */
    if (!precomp) {
        /* decode each state into its 3 digits (0..2) */
        for (int s = 0; s < STATES; ++s) {
            int d2 = s % 3;           /* column 3 (rightmost) */
            int d1 = (s / 3) % 3;     /* column 2 */
            int d0 = s / 9;           /* column 1 (leftmost) */
            row[s][0] = (unsigned char)d0;
            row[s][1] = (unsigned char)d1;
            row[s][2] = (unsigned char)d2;
        }

        /* transition: from previous row p to current row c */
        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < STATES; ++c) {
                unsigned char c0 = row[c][0], c1 = row[c][1], c2 = row[c][2];
                unsigned char p0 = row[p][0], p1 = row[p][1], p2 = row[p][2];

                /* column pair sums (j=2,3) nondecreasing downward */
                unsigned char cond1 = (c1 + c0) >= (p1 + p0);
                unsigned char cond2 = (c2 + c1) >= (p2 + p1);
                /* row pair sums (j=2,3) nondecreasing rightward */
                unsigned char cond3 = (c1 + p1) >= (c0 + p0);
                unsigned char cond4 = (c2 + p2) >= (c1 + p1);

                trans[p][c] = (cond1 && cond2 && cond3 && cond4) ? 1 : 0;
            }
        }
        precomp = 1;
    }

    /* --- dynamic programming over rows --- */
    seq_t dp[STATES], ndp[STATES];
    for (int i = 0; i < STATES; ++i)
        dp[i] = (seq_t)1;          /* row 0 can be any of the 27 states */

    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < STATES; ++i)
            ndp[i] = (seq_t)0;

        for (int p = 0; p < STATES; ++p) {
            seq_t pv = dp[p];
            if (!pv) continue;
            for (int c = 0; c < STATES; ++c) {
                if (trans[p][c])
                    ndp[c] += pv;
            }
        }
        for (int i = 0; i < STATES; ++i)
            dp[i] = ndp[i];
    }

    /* total number of arrays is sum over all final rows */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        result += dp[i];

    return result;
}
