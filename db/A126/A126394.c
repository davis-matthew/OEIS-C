/*
 * A126394  Number of base 7 n-digit numbers with adjacent digits differing by
 *          two or less.
 * https://oeis.org/A126394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of length‑n strings over digits 0…6 such that |d_i
 *            - d_{i+1}| <= 2, with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count base‑7 n‑digit numbers with adjacent digits differing by ≤2. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    static seq_t dp[7], ndp[7];    /* dp[d] = count of length‑len strings ending with digit d */

    /* Initialize for length 1: one string for each possible last digit. */
    for (int d = 0; d < 7; ++d) dp[d] = 1;

    for (idx_t len = 2; len <= n; ++len) {
        /* Compute counts for strings of current length. */
        for (int d = 0; d < 7; ++d) ndp[d] = 0;
        for (int d = 0; d < 7; ++d) {
            seq_t val = dp[d];
            if (d >= 2) ndp[d - 2] += val;
            if (d >= 1) ndp[d - 1] += val;
                ndp[d]     += val;
            if (d + 1 < 7) ndp[d + 1] += val;
            if (d + 2 < 7) ndp[d + 2] += val;
        }
        for (int d = 0; d < 7; ++d) dp[d] = ndp[d];
    }

    /* Sum over all possible final digits to obtain the total count. */
    seq_t result = 0;
    for (int d = 0; d < 7; ++d) result += dp[d];
    return result;
}
