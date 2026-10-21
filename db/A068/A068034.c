/*
 * A068034  Number of subsets of {1,2,3,...,n} that sum to 0 mod 13.
 * https://oeis.org/A068034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,…,n} whose sum is 0 modulo 13.
 *            Computes the values by an O(n) dynamic‐programming over the 13
 *            residue classes
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t dp[13] = {0};
    dp[0] = 1;                           /* empty subset */
    seq_t dp_copy[13];

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned shift = (unsigned)(i % 13);   /* how adding i changes the residue */

        /* copy current counts before they are updated */
        for (int r = 0; r < 13; ++r)
            dp_copy[r] = dp[r];

        /* add subsets that include the element i */
        for (int r = 0; r < 13; ++r)
        {
            int target = (r + shift) % 13;
            dp[target] += dp_copy[r];
        }
    }

    return dp[0];   /* subsets whose sum is ≡ 0 (mod 13) */
}
