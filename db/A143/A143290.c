/*
 * A143290  Number of binary words of length n containing at least one subword
 *          10^{10}1 and no subwords 10^{i}1 with i<10.
 * https://oeis.org/A143290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 65, u32 n <= 128, u64 n <= 258,
 *            u128 n <= 520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence/DP: a(n) = number of binary words of length n containing at least one
   subword 10^{10}1 and no subwords 10^{i}1 with i<10.  The automaton has
   12 states: 0..10 mean “after a 1 with that many consecutive zeros”,
   and 11 means “not currently after a 1”.  The DP never uses negative
   numbers, so unsigned types are safe. */

#include <string.h>

seq_t a(idx_t n)
{
    /* Automaton state definitions */
    enum { NO_AFTER_ONE = 11 };
    seq_t dp[12][2];
    seq_t next[12][2];
    int s, se, ns;

    /* Initialize DP with empty string */
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 2; ++j)
            dp[i][j] = 0;
    dp[NO_AFTER_ONE][0] = (seq_t)1;

    /* Iterate over each position of the word */
    for (idx_t i = 0; i < n; ++i) {
        /* Clear next layer */
        for (int a = 0; a < 12; ++a)
            for (int b = 0; b < 2; ++b)
                next[a][b] = 0;

        /* Transition from current DP layer */
        for (s = 0; s < 12; ++s) {
            for (se = 0; se < 2; ++se) {
                seq_t val = dp[s][se];
                if (val == 0) continue;

                /* Append a zero */
                if (s == NO_AFTER_ONE)
                    ns = NO_AFTER_ONE;
                else if (s < 10)
                    ns = s + 1;
                else /* s == 10 */
                    ns = NO_AFTER_ONE;
                next[ns][se] += val;

                /* Append a one */
                if (s == NO_AFTER_ONE) {
                    /* Start a new potential pattern */
                    ns = 0;
                    next[ns][se] += val;
                } else if (s < 10) {
                    /* Forbidden pattern: 10^i1 with i<10 → skip */
                } else { /* s == 10 */
                    ns = 0;
                    next[ns][1] += val;   /* seen the required pattern */
                }
            }
        }

        /* Move to next DP layer */
        for (int a = 0; a < 12; ++a)
            for (int b = 0; b < 2; ++b)
                dp[a][b] = next[a][b];
    }

    /* Sum counts of states that have seen the required pattern */
    seq_t res = 0;
    for (s = 0; s < 12; ++s)
        res += dp[s][1];

    return res;
}
