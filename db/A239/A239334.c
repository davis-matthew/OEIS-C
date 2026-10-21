/*
 * A239334  Number of nX2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or three plus the sum of elements
 *          above it, modulo 4.
 * https://oeis.org/A239334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×2 arrays with entries in {0,1,2,3} *
 *            such that no element equals (1 + sum of elements to its left)
 *            mod 4 * or (3 + sum of elements above it) mod 4. The sequence is
 *            computed * by dynamic programming over the cumulative column
 *            sums modulo 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   mod4_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod4_add.h"

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* the offset starts at 1 */

    /* dp[sum1*4 + sum2] – counts of partial arrays whose column‑wise
     * cumulative sums (mod 4) are (sum1, sum2).  Initially all sums are 0. */
    seq_t dp[16] = {0};
    dp[0] = 1;                         /* before any rows, sums are (0,0) */

    for (idx_t row = 1; row <= n; ++row)
    {
        seq_t next[16] = {0};

        for (unsigned s1 = 0; s1 < 4; ++s1)
        {
            for (unsigned s2 = 0; s2 < 4; ++s2)
            {
                seq_t cur = dp[s1 * 4 + s2];
                if (cur == 0) continue;

                for (unsigned a1 = 0; a1 < 4; ++a1)
                {
                    /* a1 must not be 1 or (3 + s1) mod 4  */
                    if (a1 == 1 || a1 == mod4_add(3, s1))
                        continue;

                    for (unsigned a2 = 0; a2 < 4; ++a2)
                    {
                        /* a2 must not be 1 + a1  or  (3 + s2) mod 4  */
                        if (a2 == mod4_add(1, a1) || a2 == mod4_add(3, s2))
                            continue;

                        unsigned ns1 = mod4_add(s1, a1);
                        unsigned ns2 = mod4_add(s2, a2);
                        next[ns1 * 4 + ns2] += cur;
                    }
                }
            }
        }
        /* move next to dp for the next iteration */
        for (int i = 0; i < 16; ++i)
            dp[i] = next[i];
    }

    /* sum over all final cumulative sums */
    seq_t result = 0;
    for (int i = 0; i < 16; ++i)
        result += dp[i];
    return result;
}
