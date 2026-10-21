/*
 * A210442  Number of partitions of n into proper divisors of n, cf. A027751.
 * https://oeis.org/A210442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into proper divisors of n
 *            (unrestricted partitions using only divisors of n that are < n)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 239, u64 n <= 839,
 *            u128 n <= 4679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Function must match the required signature */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate array for all proper divisors of n (max size n/2) */
    idx_t div_cap = n / 2 + 1;
    idx_t *divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
    if (!divs)
        return (seq_t)0;          /* allocation failed – unlikely for n ≤ 10000 */

    size_t dcnt = 0;
    /* Find divisors by trial up to sqrt(n) */
    for (idx_t i = (idx_t)1; (i * i) <= n; ++i) {
        if (n % i == (idx_t)0) {
            idx_t d1 = i;
            if (d1 < n && d1 > (idx_t)0 && dcnt < div_cap)
                divs[dcnt++] = d1;

            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n && d2 > (idx_t)0 && dcnt < div_cap)
                divs[dcnt++] = d2;
        }
    }

    /* Allocate temporary partition DP table of size n+1 */
    size_t dp_len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(dp_len * sizeof(seq_t));
    if (!dp) {
        free(divs);
        return (seq_t)0;          /* allocation failed */
    }

    /* Initialize DP table */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < dp_len; ++i)
        dp[i] = (seq_t)0;

    /* Standard coin–change DP: count unordered partitions */
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divs[k];
        for (size_t sum = (size_t)d; sum <= n; ++sum) {
            dp[sum] += dp[sum - (size_t)d];
        }
    }

    seq_t result = dp[(size_t)n];

    /* Clean up */
    free(dp);
    free(divs);

    return result;
}
