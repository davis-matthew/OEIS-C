/*
 * A126366  Number of base 12 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   For n ≥ 1 let dp[i] (0 ≤ i < 12) be the number of n‑digit
   base‑12 strings ending with digit i.  Initially dp[i] = 1 for all i
   (length 1 strings).  For each additional position we update with
   dp'[0]  = dp[0] + dp[1];
   dp'[11] = dp[10] + dp[11];
   dp'[i]  = dp[i-1] + dp[i] + dp[i+1]   for 1 ≤ i ≤ 10.
   After n positions a(n) = Σ_i dp[i]. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 – the empty string */
    if (n == 0)
        return 1;

    /* dp[i] holds the number of strings of the current length ending with digit i */
    seq_t dp[12];
    int i;
    for (i = 0; i < 12; ++i)
        dp[i] = 1;    /* length 1 strings */

    seq_t next[12];

    /* Iterate over the remaining positions (from length 2 up to n) */
    for (idx_t step = 2; step <= n; ++step) {
        /* Transition rule */
        next[0]  = dp[0] + dp[1];
        next[11] = dp[10] + dp[11];
        for (i = 1; i <= 10; ++i)
            next[i] = dp[i-1] + dp[i] + dp[i+1];

        /* Copy back to dp for the next iteration */
        for (i = 0; i < 12; ++i)
            dp[i] = next[i];
    }

    /* Sum all ending‑digit counts to obtain a(n) */
    seq_t total = 0;
    for (i = 0; i < 12; ++i)
        total += dp[i];

    return total;
}
