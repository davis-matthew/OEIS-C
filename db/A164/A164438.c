/*
 * A164438  Number of binary strings of length n with no substrings equal to
 *          0000 0110 or 0111.
 * https://oeis.org/A164438
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substring
 *            equal to 0000, 0110, or 0111. Computation via a DFA with 6
 *            states (exclude forbidden state)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* States represented by the longest suffix that is a prefix of a forbidden
       pattern:
         0: ""          (empty)
         1: "0"
         2: "00"
         5: "000"
         3: "01"
         4: "011"
       Transitions are pre-computed manually; forbidden transitions are ignored. */
    if (n < 4) {
        /* behaviour below offset does not matter; return 0 */
        return (seq_t)0;
    }

    /* Current number of strings for each state */
    seq_t dp[6] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0 };
    dp[0] = (seq_t)1;          /* empty string */

    for (idx_t i = 0; i < n; ++i) {
        seq_t ndp[6] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0 };

        /* Transitions from state 0 ("") */
        ndp[1] += dp[0];      /* add 0 => state 1 ("0")   */
        ndp[0] += dp[0];      /* add 1 => state 0 ("" )    */

        /* Transitions from state 1 ("0") */
        ndp[2] += dp[1];      /* add 0 => state 2 ("00")  */
        ndp[3] += dp[1];      /* add 1 => state 3 ("01")  */

        /* Transitions from state 2 ("00") */
        ndp[5] += dp[2];      /* add 0 => state 5 ("000") */
        ndp[3] += dp[2];      /* add 1 => state 3 ("01")  */

        /* Transitions from state 5 ("000") */
        /* add 0 -> forbidden, ignored */
        ndp[3] += dp[5];      /* add 1 => state 3 ("01")  */

        /* Transitions from state 3 ("01") */
        ndp[1] += dp[3];      /* add 0 => state 1 ("0")   */
        ndp[4] += dp[3];      /* add 1 => state 4 ("011") */

        /* Transitions from state 4 ("011") */
        /* both add 0 or 1 lead to forbidden, ignored */

        /* prepare for next iteration */
        dp[0] = ndp[0];
        dp[1] = ndp[1];
        dp[2] = ndp[2];
        dp[3] = ndp[3];
        dp[4] = ndp[4];
        dp[5] = ndp[5];
    }

    /* Sum all non-forbidden states */
    seq_t total = dp[0];
    total += dp[1];
    total += dp[2];
    total += dp[3];
    total += dp[4];
    total += dp[5];
    return total;
}
