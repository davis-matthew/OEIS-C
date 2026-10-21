/*
 * A225021  Number of (n+1)X3 0..2 matrices with each 2X2 permanent equal.
 * https://oeis.org/A225021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 matrices with entries 0–2 in which
 *            every 2×2 permanent is equal to a common value
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a transfer‑matrix for the 27 possible
   rows (3 digits in base 3).  For each ordered pair of rows the
   common permanent value C is computed.  Sequences of rows are then
   counted by dynamic programming, maintaining the last row and the
   common value C.  Complexity is O(n·27·27) which is well below the
   required 2 s limit for n up to about 10 000. */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Precompute all 27 rows (0–26), stored as three base‑3 digits. */
    static int row[27][3];
    static int once = 0;
    if (!once) {
        for (int i = 0; i < 27; ++i) {
            row[i][0] = i / 9;          /* highest digit            */
            row[i][1] = (i / 3) % 3;    /* middle digit             */
            row[i][2] = i % 3;          /* lowest digit             */
        }
        once = 1;
    }

    /* Precompute allowed transitions:  if |E1−E2|=0 then store the common value C,
       otherwise store -1.  C ranges from 0 to 8. */
    static int8_t allowed[27][27];
    static int once2 = 0;
    if (!once2) {
        for (int r = 0; r < 27; ++r) {
            for (int s = 0; s < 27; ++s) {
                int e1 = row[r][0] * row[s][1] + row[r][1] * row[s][0];
                int e2 = row[r][1] * row[s][2] + row[r][2] * row[s][1];
                allowed[r][s] = (e1 == e2) ? (int8_t)e1 : (int8_t)-1;
            }
        }
        once2 = 1;
    }

    /* DP arrays: dp_cur and dp_next hold counts for sequences of a given
       length, indexed by last row (0–26) and constant value C (0–8). */
    static seq_t dp_cur[27][9];
    static seq_t dp_next[27][9];

    /* Initialise for sequences of length 2 (n=1).  Each ordered pair
       of rows that yields a common permanent C contributes one matrix. */
    memset(dp_cur, 0, sizeof(dp_cur));
    for (int r = 0; r < 27; ++r) {
        for (int s = 0; s < 27; ++s) {
            int8_t c = allowed[r][s];
            if (c >= 0) {
                dp_cur[s][c] += (seq_t)1;   /* count the pair (r,s) */
            }
        }
    }

    /* If n==1, we are already done. */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 27; ++r)
            for (int c = 0; c < 9; ++c)
                total += dp_cur[r][c];
        return total;
    }

    /* Extend the sequences one row at a time until we have n+1 rows. */
    for (idx_t step = 3; step <= n + 1; ++step) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int r = 0; r < 27; ++r) {
            for (int c = 0; c < 9; ++c) {
                seq_t val = dp_cur[r][c];
                if (val == 0)
                    continue;
                /* Try all possible next rows s. */
                for (int s = 0; s < 27; ++s) {
                    if (allowed[r][s] == c)
                        dp_next[s][c] += val;
                }
            }
        }
        /* Move dp_next into dp_cur for the next iteration. */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    /* Sum all counts for the final length n+1. */
    seq_t total = 0;
    for (int r = 0; r < 27; ++r)
        for (int c = 0; c < 9; ++c)
            total += dp_cur[r][c];
    return total;
}
