/*
 * A126522  Number of base 27 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A126522: Number of base‑27 n‑digit numbers with adjacent digits differing by ≤4.
   Recurrence: for n ≥ 1
      count(1,d) = 1          (for all digits d=0…26)
      count(k+1, d) = Σ_{j=|d−j|≤4} count(k, j)
   Then a(n) = Σ_{d=0}^{26} count(n, d)
   Base: a(0) = 1 (empty string)
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 0 return 1 */
    if (n == 0) return (seq_t)1;

    /* There are 27 digits: 0 … 26 */
    const idx_t D = 27;

    /* dp[d] = number of length‑len sequences ending with digit d */
    seq_t dp[D], nxt[D];
    for (idx_t d = 0; d < D; ++d) {
        dp[d] = (seq_t)1; /* length 1 */
    }

    /* Build up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < D; ++d) {
            seq_t sum = (seq_t)0;
            idx_t low = (d >= 4) ? d - 4 : 0;
            idx_t high = (d + 4 < D) ? d + 4 : D - 1;
            for (idx_t j = low; j <= high; ++j) {
                sum += dp[j];
            }
            nxt[d] = sum;
        }
        /* move nxt to dp for next iteration */
        for (idx_t d = 0; d < D; ++d) dp[d] = nxt[d];
    }

    /* Sum all ending counts to get total sequences of length n */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < D; ++d) total += dp[d];
    return total;
}
