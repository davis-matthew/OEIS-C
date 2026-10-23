/*
 * A126539  Number of base 18 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the number of length‑n base‑18 strings
   whose adjacent digits differ by at most 5.
   For n = 0, a(0) = 1 (the empty string).
   For n ≥ 1, let dp[d] be the number of such strings of current length
   ending with digit d (0 ≤ d < 18).  Initially dp[d] = 1 for all d
   (length 1).  For each additional position,
   ndp[d] = Σ_{x: |x-d|≤5} dp[x].
   After processing n positions, a(n) = Σ_{d} dp[d].
   This runs in O(n) time and O(18) space. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;

    seq_t dp[18], ndp[18];
    for (unsigned d = 0; d < 18; ++d) dp[d] = (seq_t)1;

    for (idx_t len = 2; len <= n; ++len)
    {
        for (unsigned d = 0; d < 18; ++d)
        {
            seq_t sum = 0;
            unsigned lo = (d >= 5) ? d - 5 : 0;
            unsigned hi = (d + 5 <= 17) ? d + 5 : 17;
            for (unsigned x = lo; x <= hi; ++x)
                sum = sum + dp[x];
            ndp[d] = sum;
        }
        /* copy ndp into dp */
        for (unsigned d = 0; d < 18; ++d) dp[d] = ndp[d];
    }

    seq_t total = 0;
    for (unsigned d = 0; d < 18; ++d) total = total + dp[d];
    return total;
}
