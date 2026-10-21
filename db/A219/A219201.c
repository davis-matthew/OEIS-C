/*
 * A219201  Number of partitions of n into 7 distinct primes.
 * https://oeis.org/A219201
 *
 * Offset:    58    (a(58) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 400, u32 n <= 3526, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of partitions of n into 7 distinct primes.
   Implements a straightforward dynamic‑programming algorithm
   using the helper build_primes_upto to generate all primes <= n.
   Complexity:  O(#primes*n*7), easily fast for n <= 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* The function is thread‑unsafe due to static allocation, but the
   build harness does not require thread safety. */
seq_t a(idx_t n)
{
    /* OEIS offset 58 */
    if (n < 58) {
        return 0;
    }

    /* Build list of primes up to n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto((idx_t)n, &primes, &primes_cnt);

    size_t sz = (size_t)n + 1;          /* array length */
    if (sz == 0) {                     /* overflow guard */
        return 0;
    }

    /* Allocate DP tables for k = 0..7 */
    seq_t *dp[8];
    for (int k = 0; k < 8; k++) {
        dp[k] = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp[k]) {
            /* Allocation failed – clean up and return 0 */
            for (int j = 0; j < k; j++) {
                free(dp[j]);
            }
            free(primes);
            return 0;
        }
    }

    /* Base case: 0 primes sum to 0 */
    dp[0][0] = (seq_t)1;

    /* DP transition: for each prime, update counts in reverse k ordering */
    for (size_t pi = 0; pi < primes_cnt; pi++) {
        uint64_t p = primes[pi];
        if (p > (uint64_t)n) {
            break;
        }
        /* For each k from 6 down to 0, add contributions to dp[k+1] */
        for (int k = 6; k >= 0; k--) {
            idx_t max_s = (idx_t)n - (idx_t)p;
            for (idx_t s = 0; s <= max_s; s++) {
                dp[k + 1][s + (idx_t)p] += dp[k][s];
            }
        }
    }

    seq_t result = dp[7][(size_t)n];

    /* Free allocated memory */
    for (int k = 0; k < 8; k++) {
        free(dp[k]);
    }
    free(primes);

    return result;
}
