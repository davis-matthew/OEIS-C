/*
 * A035362  Number of partitions of n into parts 4k or 4k+1.
 * https://oeis.org/A035362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Calculation of a(n): number of partitions of n into parts 4k or 4k+1
   (OEIS A035362).  The function uses a classic unbounded coin‑change
   dynamic programming algorithm:  for each admissible part size
   add partitions that end with that part.
   Complexity: O(n · (#admissible parts)) ≈ O(n²/2), fast enough for n up to ~10⁴.
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n is at least 1 in the OEIS range; for safety handle n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Allocate DP table of size n+1; initialized to 0 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure – unlikely in test harness */
    memset(dp, 0, (size_t)(n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* Process parts of the form 4k+1 (including 1 itself) */
    for (idx_t k = 0; ; ++k) {
        idx_t part = 4 * k + 1;
        if (part > n)
            break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* Process parts of the form 4k (k >= 1) */
    for (idx_t k = 1; ; ++k) {
        idx_t part = 4 * k;
        if (part > n)
            break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
