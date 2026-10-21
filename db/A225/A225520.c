/*
 * A225520  The number of subsets of the set of divisors of n in which
 *          elements are pairwise coprime.
 * https://oeis.org/A225520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of the divisors of n in which all
 *            elements are pairwise coprime
 * Range:     u8 n <= 359, u16 n <= 359, u32 n <= 359, u64 n <= 359,
 *            u128 n <= 359, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper functions and types are assumed to be provided externally. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Special case n == 0 is undefined; all tests use n >= 1. */
    uint64_t x = (uint64_t)n;

    /* Factor n into distinct primes and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    if (cnt == 0) {                /* n == 1 */
        return (seq_t)2;           /* { } and {1} */
    }

    /* Number of distinct primes is cnt.  Compute 2^cnt. */
    size_t k = cnt;
    size_t M = (size_t)1 << k;     /* Safe: k is small for all n in tests. */

    /* Allocate DP arrays. */
    seq_t *dp = (seq_t*)malloc(M * sizeof(seq_t));
    seq_t *weight = (seq_t*)malloc(M * sizeof(seq_t));
    if (!dp || !weight) {
        /* In case of allocation failure, return 0 (should not happen). */
        if (dp) free(dp);
        if (weight) free(weight);
        return (seq_t)0;
    }

    /* Initialize DP. */
    for (size_t i = 0; i < M; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Precompute weight for each non‑empty support subset. */
    weight[0] = (seq_t)0; /* unused */
    for (size_t s = 1; s < M; ++s) {
        seq_t w = (seq_t)1;
        for (size_t j = 0; j < k; ++j) {
            if (s & ((size_t)1 << j))
                w *= (seq_t)exps[j];
        }
        weight[s] = w;
    }

    /* DP over supports.  Each support can be used at most once. */
    for (size_t s = 1; s < M; ++s) {
        seq_t w = weight[s];
        for (size_t mask = M; mask-- > 0; ) {
            if ((mask & s) == 0) {
                dp[mask | s] += dp[mask] * w;
            }
        }
    }

    /* Sum all DP states and account for the divisor 1. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < M; ++i)
        total += dp[i];
    total *= (seq_t)2;                 /* 1 can be either chosen or not. */

    free(dp);
    free(weight);
    return total;
}
