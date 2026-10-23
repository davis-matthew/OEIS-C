/*
 * A116375  Number of partitions of n into parts with digital root = 5.
 * https://oeis.org/A116375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with digital root 5
 *            (i.e. parts congruent to 5 modulo 9)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Allocate DP array of size n+1.  dp[0] = 1 (empty partition). */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure – caller will see 0 */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Classic 1‑D coin‑change DP:  parts = 5,14,23,... ≤ n */
    for (idx_t part = 5; part <= n; part += 9) {
        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] += dp[sum - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
