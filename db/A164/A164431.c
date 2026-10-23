/*
 * A164431  Number of binary strings of length n with no substrings equal to
 *          0000, 0011 or 1101.
 * https://oeis.org/A164431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164431: number of binary strings of length n with no substrings
   0000, 0011 or 1101.
   Computation is done by dynamic programming on a DFA derived from the
   forbidden patterns. */

seq_t a(idx_t n)
{
    /* DFA states : 0..10
       0 : ""          (empty)
       1 : "0"
       2 : "00"
       3 : "000"
       4 : "0000"  (forbidden)
       5 : "001"
       6 : "0011"  (forbidden)
       7 : "1"
       8 : "11"
       9 : "110"
       10: "1101" (forbidden)
    */
    static const int trans[11][2] = {
        /* 0 */ { 1, 7 },   /* 1 */
        /* 1 */ { 2, 7 },   /* 2 */
        /* 2 */ { 3, 5 },   /* 3 */
        /* 3 */ { -1, 5 },  /* 4 */
        /* 4 */ { -1, -1 }, /* 5 */
        /* 5 */ { 1, -1 },  /* 6 */
        /* 6 */ { -1, -1 }, /* 7 */
        /* 7 */ { 1, 8 },   /* 8 */
        /* 8 */ { 9, 8 },   /* 9 */
        /* 9 */ { 2, -1 },  /*10 */
        /*10 */ { -1, -1 }  /*11 (unused) */
    };

    /* transitions from forbidden states are never taken (t==-1) */
    seq_t dp[11] = { 0 };
    seq_t next[11] = { 0 };
    dp[0] = 1;          /* start from the empty string */

    for (idx_t step = n; step-- > 0;) {
        /* clear next array */
        for (int i = 0; i < 11; ++i)
            next[i] = 0;

        for (int s = 0; s < 11; ++s) {
            seq_t val = dp[s];
            if (val == 0)
                continue;
            /* input bit 0 */
            int t = trans[s][0];
            if (t >= 0)
                next[t] += val;
            /* input bit 1 */
            t = trans[s][1];
            if (t >= 0)
                next[t] += val;
        }
        /* copy next to dp */
        for (int i = 0; i < 11; ++i)
            dp[i] = next[i];
    }

    /* sum all non‑forbidden states */
    seq_t result = 0;
    for (int i = 0; i < 11; ++i)
        result += dp[i];
    return result;
}
