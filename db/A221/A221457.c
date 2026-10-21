/*
 * A221457  Number of 0..6 arrays of length n with each element unequal to at
 *          least one neighbor, with new values introduced in 0..6 order.
 * https://oeis.org/A221457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..6 arrays of length n such that each element
 *            is unequal to at least one neighbor and new values are
 *            introduced in increasing order. Computed by dynamic programming
 *            with states (current maximum value m, last run length r ∈ {1,2})
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n ≥ 1
   a(1) = 0
   a(2) = 1
   a(3) = 2, ... as given in the OEIS entry. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)        /* a(1) = 0 by definition */
        return 0;

    /* DP arrays: dp_cur[m][r] where
         m : current maximum used value (0..6)
         r : last run length (0 → length 1, 1 → length 2)   */
    seq_t dp_cur[7 + 1][2];
    seq_t dp_next[7 + 1][2];

    /* initialize to zero */
    for (idx_t m = 0; m <= 6; ++m)
        for (idx_t r = 0; r < 2; ++r)
            dp_cur[m][r] = 0;

    /* start with first element 0 : max value 0 , run length 1 */
    dp_cur[0][0] = 1;

    for (idx_t pos = 1; pos < n; ++pos) {
        /* clear next state */
        for (idx_t m = 0; m <= 6; ++m)
            for (idx_t r = 0; r < 2; ++r)
                dp_next[m][r] = 0;

        for (idx_t m = 0; m <= 6; ++m) {
            /* state with last run length 1 */
            seq_t cur1 = dp_cur[m][0];
            if (cur1) {
                /* same value allowed only when pos != 1 (first two elements cannot be equal) */
                if (pos != 1)
                    dp_next[m][1] += cur1;          /* run length becomes 2 */

                if (m < 6)                            /* introduce new value */
                    dp_next[m + 1][0] += cur1;        /* run length 1 */

                if (m > 0)                            /* choose an old value different from previous */
                    dp_next[m][0] += cur1 * (seq_t)m;
            }

            /* state with last run length 2 */
            seq_t cur2 = dp_cur[m][1];
            if (cur2) {
                if (m < 6)                            /* new value */
                    dp_next[m + 1][0] += cur2;

                if (m > 0)                            /* old value different from previous */
                    dp_next[m][0] += cur2 * (seq_t)m;
            }
        }

        /* move next state to current */
        for (idx_t m = 0; m <= 6; ++m)
            for (idx_t r = 0; r < 2; ++r)
                dp_cur[m][r] = dp_next[m][r];
    }

    /* answer is the number of sequences whose last run length is 1 */
    seq_t result = 0;
    for (idx_t m = 0; m <= 6; ++m)
        result += dp_cur[m][0];

    return result;
}
