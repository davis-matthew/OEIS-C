/*
 * A066874  Number of partitions of n into unitary divisors of n.
 * https://oeis.org/A066874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 173, u32 n <= 329, u64 n <= 2309,
 *            u128 n <= 8612, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = number of partitions of n into its unitary divisors
   via dynamic programming */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Handle trivial case (n < 1) safely – not used per problem statement */
    if (n == 0) return 0;

    /* Factor n into prime powers */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t omega = 0;
    factor_u64(n, primes, exps, &omega);

    /* Allocate array for all unitary divisors */
    size_t unitary_cap = (omega == 0) ? 1ULL : (size_t)1ULL << omega; /* 2^omega */
    idx_t *unitary = (idx_t*)malloc(unitary_cap * sizeof(idx_t));
    if (!unitary) return (seq_t)0;   /* Allocation failure – return 0 */
    unitary[0] = 1;
    size_t ucnt = 1;

    /* Generate each unitary divisor as product of a subset of prime powers */
    for (size_t i = 0; i < omega; ++i) {
        /* Compute p^e */
        idx_t power = 1;
        for (size_t k = 0; k < exps[i]; ++k) {
            power *= (idx_t)primes[i];
        }
        size_t old = ucnt;
        for (size_t j = 0; j < old; ++j) {
            unitary[ucnt++] = unitary[j] * power;
        }
    }

    /* DP array for partitions: dp[s] = number of partitions of s */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) {
        free(unitary);
        return (seq_t)0;   /* Allocation failure – return 0 */
    }
    for (idx_t s = 0; s <= n; ++s) dp[s] = 0;
    dp[0] = 1;

    /* For each unitary divisor, update DP (orderless partitions) */
    for (size_t i = 0; i < ucnt; ++i) {
        idx_t d = unitary[i];
        for (idx_t s = d; s <= n; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t ans = dp[n];

    /* Clean up */
    free(dp);
    free(unitary);

    return ans;
}
