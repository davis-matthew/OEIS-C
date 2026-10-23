/*
 * A118430  Number of binary sequences of length n containing exactly one
 *          subsequence 010.
 * https://oeis.org/A118430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 36, u64 n <= 74, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP over an automaton that tracks the last two bits and one
   occurrence of the pattern 010.  The DP counts binary sequences
   of length n that contain exactly one occurrence of the substring
   "010". */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Transition table for the automaton:
       state 0: no prefix matched
       state 1: suffix "0" matched
       state 2: suffix "01" matched
       Reading a bit from a state yields the next state. */
    static const int trans[3][2] = {
        {1, 0},   /* from state 0 with bit 0 or 1 */
        {1, 2},   /* from state 1 with bit 0 or 1 */
        {1, 0}    /* from state 2 with bit 0 or 1 */
    };

    /* dpCurr[s][t] and dpNext[s][t] where
         s : current automaton state (0..2)
         t : number of occurrences seen (0 or 1).   */
    seq_t dpCurr[3][2] = {0};
    seq_t dpNext[3][2] = {0};

    dpCurr[0][0] = 1;   /* empty string: state 0, 0 occurrences */

    for (idx_t i = 0; i < n; ++i) {
        /* clear dpNext */
        dpNext[0][0] = dpNext[0][1] = 0;
        dpNext[1][0] = dpNext[1][1] = 0;
        dpNext[2][0] = dpNext[2][1] = 0;

        for (int s = 0; s < 3; ++s) {
            for (int t = 0; t <= 1; ++t) {
                seq_t cur = dpCurr[s][t];
                if (cur == 0)
                    continue;
                /* try adding bit 0 */
                {
                    int ns = trans[s][0];
                    int add = (s == 2) ? 1 : 0; /* new occurrence if state 2 on bit 0 */
                    int nt = t + add;
                    if (nt <= 1)
                        dpNext[ns][nt] += cur;
                }
                /* try adding bit 1 */
                {
                    int ns = trans[s][1];
                    int nt = t; /* no new occurrence on bit 1 */
                    dpNext[ns][nt] += cur;
                }
            }
        }
        /* swap dpCurr and dpNext */
        dpCurr[0][0] = dpNext[0][0];
        dpCurr[0][1] = dpNext[0][1];
        dpCurr[1][0] = dpNext[1][0];
        dpCurr[1][1] = dpNext[1][1];
        dpCurr[2][0] = dpNext[2][0];
        dpCurr[2][1] = dpNext[2][1];
    }

    /* Sum over all end states with exactly one occurrence. */
    seq_t ans = 0;
    ans += dpCurr[0][1];
    ans += dpCurr[1][1];
    ans += dpCurr[2][1];
    return ans;
}
