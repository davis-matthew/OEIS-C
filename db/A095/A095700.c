/*
 * A095700  Expansion of 1 + Sum_{i>=1} (x^prime(i)/Product_{j=1..i} (1-x^j)).
 * https://oeis.org/A095700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, and for n>0 * a(n) = Σ_{i:prime_i ≤ n} p(n-prime_i, i),
 *            * where p(m, k) is the number of partitions of m using parts ≤ k
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

seq_t a(idx_t n)
{
    /* Allocate DP arrays */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *aarr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp || !aarr) {
        free(dp);
        free(aarr);
        return (seq_t)0; /* allocation failure */
    }

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
        aarr[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;     /* one way to partition 0 */
    aarr[0] = (seq_t)1;   /* a(0) = 1 */

    /* Generate primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Main DP loop */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        /* Update dp for new part size = idx + 1 */
        idx_t part = (idx_t)(idx + 1);
        if (part > n) break;  /* further parts are useless */
        for (idx_t m = part; m <= n; ++m) {
            dp[m] += dp[m - part];
        }

        idx_t prime = (idx_t)primes[idx];
        if (prime > n) break; /* no further contributions */
        for (idx_t m = prime; m <= n; ++m) {
            aarr[m] += dp[m - prime];
        }
    }

    /* Clean up */
    seq_t result = aarr[n];
    free(dp);
    free(aarr);
    free(primes);

    return result;
}
