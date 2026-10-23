/*
 * A170511  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            with 22 generators satisfying (S_i)^2 = (S_i S_j)^46 = I.
 *            Recurrence: For n >= 2, let f[l] (2 <= l <= 91) be the number of
 *            reduced words of the current length whose last l letters form an
 *            alternating suffix of two distinct generators...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only standard headers needed for the implementation. */
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 22;
    if (n == 2) return 462;   /* 22 * 21 */

    /* States for alternating suffix lengths 2 … 91. */
    seq_t dp[92] = {0};
    dp[2] = (seq_t)22 * 21;   /* initial words of length 2 */

    for (idx_t len = 3; len <= n; ++len)
    {
        /* Sum of all states. */
        seq_t total = 0;
        for (idx_t l = 2; l <= 91; ++l) total += dp[l];

        /* Compute next states. */
        seq_t newdp[92] = {0};

        newdp[2] = 20 * total;   /* break the pattern */

        for (idx_t l = 2; l <= 90; ++l) {
            newdp[l + 1] = dp[l];   /* continue alternating */
        }
        newdp[91] = dp[90];        /* last allowed length */

        /* Copy newdp into dp for next iteration. */
        for (idx_t l = 2; l <= 91; ++l) dp[l] = newdp[l];
    }

    /* Sum over all valid states gives a(n). */
    seq_t result = 0;
    for (idx_t l = 2; l <= 91; ++l) result += dp[l];
    return result;
}
