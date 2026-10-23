/*
 * A035379  Number of partitions of n into parts 6k or 6k+5.
 * https://oeis.org/A035379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are 6k or 6k+5.
 *            Computation: dynamic programming (unbounded coin change)
 * Range:     u8 n <= 69, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* offset 1 – for n = 0 return 0 (undefined by OEIS) */
    if (n == 0) return 0;

    /* static state: dp table and largest n that has been computed */
    static seq_t *dp = NULL;
    static idx_t cur_max = 0;

    /* Allocate or resize table if needed, then rebuild it from scratch */
    if (n > cur_max) {
        idx_t new_size = n + 1;          /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        dp = tmp;

        /* zero all entries first */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;                       /* empty partition */

        /* process each allowed part size */
        for (idx_t part = 5; part <= n; ++part) {
            if ((part % 6ULL) == 0ULL || (part % 6ULL) == 5ULL) {
                for (idx_t sum = part; sum <= n; ++sum) {
                    dp[sum] += dp[sum - part];
                }
            }
        }
        cur_max = n;
    }

    return dp[n];
}
