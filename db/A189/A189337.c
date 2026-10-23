/*
 * A189337  Number of nX3 binary arrays without the pattern 1 0 0 1
 *          diagonally, vertically, antidiagonally or horizontally.
 * https://oeis.org/A189337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (c(n))³ where c(n) is the number of binary strings of
 *            length n that avoid the substring “1001”. c(n) is obtained by a
 *            simple automaton with 4 states (length of the longest prefix of
 *            “1001” that matches a suffix of the current string). Each state
 *            transition is defined by the table below, wh...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool (not used in the core, but required) */

seq_t a(idx_t n)
{
    /* Transition table for the automaton recognizing the suffix of
       the pattern “1001”.  trans[s][b] gives the new state after
       appending bit b (0 or 1) to a string whose current suffix
       matches the first s bits of the pattern.  A value of 4 indicates
       that the forbidden substring has been formed (not allowed). */
    static const int trans[4][2] = {
        /* state 0: no prefix matched */
        {0, 1},   /* b=0 -> state 0,   b=1 -> state 1 */
        /* state 1: matched "1"    */
        {2, 1},   /* b=0 -> state 2,   b=1 -> state 1 */
        /* state 2: matched "10"   */
        {3, 1},   /* b=0 -> state 3,   b=1 -> state 1 */
        /* state 3: matched "100"  */
        {0, 4}    /* b=0 -> state 0,   b=1 -> forbidden (state 4) */
    };

    /* dp[s] counts the number of valid strings of the current length
       that end in state s.  nxt[s] is used for the next length. */
    seq_t dp[4] = {0};
    seq_t nxt[4] = {0};

    dp[0] = 1;  /* empty string – matches 0 characters of the pattern */

    for (idx_t i = 0; i < n; ++i) {
        /* zero next array */
        nxt[0] = nxt[1] = nxt[2] = nxt[3] = 0;

        for (int s = 0; s < 4; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;

            int ns = trans[s][0];
            if (ns < 4) nxt[ns] += val;   /* append 0 */

            ns = trans[s][1];
            if (ns < 4) nxt[ns] += val;   /* append 1 */
        }

        /* copy nxt back into dp for the next iteration */
        dp[0] = nxt[0];
        dp[1] = nxt[1];
        dp[2] = nxt[2];
        dp[3] = nxt[3];
    }

    /* c(n) is the total number of strings of length n that avoid “1001” */
    seq_t c = dp[0] + dp[1] + dp[2] + dp[3];

    /* a(n) = c(n)^3 */
    seq_t result = c * c;
    result *= c;

    return result;
}
