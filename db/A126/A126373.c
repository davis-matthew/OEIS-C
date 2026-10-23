/*
 * A126373  Number of base 19 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1,   a(1) = 19.
   For n ≥ 2 let dp[d] be the number of n‑digit base‑19 numbers
   ending with digit d (0 ≤ d < 19) whose adjacent digits differ
   by at most 1.  Then
       dp_next[d] = dp[d] + (d>0   ? dp[d-1] : 0)
                     + (d<18 ? dp[d+1] : 0);
   and a(n) is the sum of all dp_next[d].
   This algorithm runs in O(n) time and O(1) extra space. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;          /* a(0) */
    if (n == (idx_t)1) return (seq_t)19;         /* a(1) */

    /* dp[d] holds counts for the current length; initialize for length 1 */
    seq_t dp[19];
    for (int d = 0; d < 19; ++d)
        dp[d] = (seq_t)1;      /* a single digit is always valid */

    seq_t total = 0;
    seq_t next[19];

    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 19; ++d) {
            seq_t sum = dp[d];
            if (d > 0)   sum += dp[d - 1];
            if (d < 18)  sum += dp[d + 1];
            next[d] = sum;
        }
        /* compute total for this length */
        total = (seq_t)0;
        for (int d = 0; d < 19; ++d)
            total += next[d];

        /* prepare for next iteration */
        for (int d = 0; d < 19; ++d)
            dp[d] = next[d];
    }

    return total;   /* a(n) */
}
