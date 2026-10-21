/*
 * A219182  Maximal number of partitions of n into any number k of distinct
 *          prime parts or 0 if there are no such partitions.
 * https://oeis.org/A219182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k>=0} A219180(n,k) A219190(n,k) counts the
 *            partitions of n into exactly k distinct primes. The
 *            implementation builds primes up to n, then uses a
 *            two‑dimensional dynamic programming table dp[k][s] that stores
 *            the number of subsets of size k whose prime parts sum to s. The
 *            answer for n...
 * Range:     u8 n <= 142, u16 n <= 377, u32 n <= 1185, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Build all primes <= n */
    size_t prime_cnt;
    uint64_t *primes;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Compute the maximum k such that the sum of the smallest k primes
       does not exceed n.  This is an upper bound on the number of parts
       that can appear in any partition of n. */
    size_t max_k_needed = 0;
    idx_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (sum + primes[i] <= n) {
            sum += primes[i];
            ++max_k_needed;
        } else
            break;
    }

    /* Allocate DP table: dp[k][s] = number of subsets of k distinct primes
       whose sum is s.  Rows are indexed by k from 0..max_k_needed. */
    size_t cap_k = max_k_needed + 1;   /* include k = 0 */
    size_t cap_n = n + 1;
    seq_t *dp = (seq_t *)malloc(cap_k * cap_n * sizeof(seq_t));
    if (!dp) {
        free(primes);
        return (seq_t)0;
    }
    for (size_t i = 0; i < cap_k * cap_n; ++i)
        dp[i] = 0;
    dp[0] = 1;                       /* dp[0][0] = 1 */

    /* Knapsack‑type DP over all primes.  For each prime p we update
       rows in descending k to avoid reuse of the same prime. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        size_t cur_max_k = (pi + 1 <= max_k_needed) ? (pi + 1) : max_k_needed;
        for (size_t k = cur_max_k; k > 0; --k) {
            seq_t *cur_row = dp + k * cap_n;
            seq_t *prev_row = dp + (k - 1) * cap_n;
            for (idx_t s = n; s >= p; --s)
                cur_row[s] += prev_row[s - p];
        }
    }

    /* Find the maximum count among all k for the target sum n. */
    seq_t best = 0;
    for (size_t k = 1; k <= max_k_needed; ++k) {
        seq_t val = dp[k * cap_n + n];
        if (val > best)
            best = val;
    }

    free(dp);
    free(primes);
    return best;
}
