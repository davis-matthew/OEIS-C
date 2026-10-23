/*
 * A024939  Number of partitions of n into distinct odd primes.
 * https://oeis.org/A024939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   partitions of n into distinct odd primes
 * Range:     u8 n <= 125, u16 n <= 341, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Number of partitions of n into distinct odd primes. */
seq_t a(idx_t n)
{
    /* Base case: one way to partition 0 (empty partition). */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate DP array of size n+1. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) {
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i) {
        dp[i] = (seq_t)0;
    }

    /* Get list of primes up to n. */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Update DP for each odd prime (distinct parts). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)prime_list[i];
        if (p <= 2) {   /* Skip 2, the only even prime. */
            continue;
        }
        for (idx_t s = n; s >= p; --s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[n];

    /* Free allocated memory. */
    free(dp);
    free(prime_list);

    return result;
}
