/*
 * A164444  Number of binary strings of length n with no substrings equal to
 *          0000 1001 or 1011.
 * https://oeis.org/A164444
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 51, u64 n <= 104,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164444: Number of binary strings of length n with no
   substrings equal to 0000, 1001, or 1011.
   Computed by dynamic programming on a DFA with 8 states. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If the requested index is less than the offset we return
       anything – the behaviour below the offset is not checked. */
    if (n == (idx_t)0) return 0;

    /* DFA states:
       0: ""      (empty)
       1: "0"
       2: "00"
       3: "000"
       4: "1"
       5: "10"
       6: "100"
       7: "101"
    */
    static const int trans[8][2] = {
        /* state 0 */  {1, 4},
        /* state 1 */  {2, 4},
        /* state 2 */  {3, 4},
        /* state 3 */  {-1, 4},   /* 0000 forbidden on 0 */
        /* state 4 */  {5, 4},
        /* state 5 */  {6, 7},
        /* state 6 */  {3, -1},   /* 1001 forbidden on 1 */
        /* state 7 */  {5, -1}    /* 1011 forbidden on 1 */
    };

    seq_t dp_cur[8]  = {0};
    seq_t dp_next[8] = {0};

    dp_cur[0] = 1;       /* empty string of length 0 */

    for (idx_t len = 1; len <= n; ++len)
    {
        for (int i = 0; i < 8; ++i) dp_next[i] = 0;

        for (int s = 0; s < 8; ++s)
        {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            int t0 = trans[s][0];
            if (t0 >= 0) dp_next[t0] += val;

            int t1 = trans[s][1];
            if (t1 >= 0) dp_next[t1] += val;
        }

        for (int i = 0; i < 8; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 8; ++i) total += dp_cur[i];
    return total;
}
