/*
 * A211111  Number of partitions of n into distinct divisors > 1 of n.
 * https://oeis.org/A211111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct divisors > 1 of n
 * Range:     u8 n <= 179, u16 n <= 719, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Maximum number of proper divisors of n <= 10000 is far less than 256 */
    static const size_t MAX_D = 256;
    idx_t divisors[MAX_D];
    size_t dcnt = 0;

    /* Collect all divisors > 1 of n */
    if (n > 1) {
        for (idx_t i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                if (dcnt < MAX_D) divisors[dcnt++] = i;
                idx_t j = n / i;
                if (j != i && dcnt < MAX_D) divisors[dcnt++] = j;
            }
        }
        /* n itself is a divisor > 1 */
        if (dcnt < MAX_D) divisors[dcnt++] = n;
    }

    /* Subset‑sum dynamic programming: dp[s] = number of ways to reach sum s */
    seq_t *dp = NULL;
#if defined(__cplusplus)
    dp = new seq_t[(size_t)(n + 1)];
#else
    dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
    if (dp == NULL) return (seq_t)0;  /* allocation failure */

    /* Initialise */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Account each divisor exactly once (distinct divisors) */
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divisors[k];
        for (idx_t s = n; s >= d; --s) {
            dp[s] = dp[s] + dp[s - d];
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
