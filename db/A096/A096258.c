/*
 * A096258  Number of partitions of n into distinct nonprime parts.
 * https://oeis.org/A096258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into distinct nonprime parts
 * Range:     u8 n <= 52, u16 n <= 127, u32 n <= 356, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over all nonprime integers ≤ n with a fresh sieve each call. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Convert n to a size_t for array indexing.  n is guaranteed to fit in size_t up to the limits of the test harness. */
    size_t N = (size_t)n;

    /* Allocate DP array for sums 0..N. */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= N; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                   /* one way to obtain the sum 0 */

    /* Sieve of Eratosthenes: is_prime[i] == true  ⇒ i is prime.  Nonprime parts are those with !is_prime[i]. */
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    for (size_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;
    for (size_t i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* DP over distinct nonprime parts.  For each part v we add its contribution by iterating the sums backwards. */
    for (size_t v = 1; v <= N; ++v) {
        if (!is_prime[v]) {                     /* v is a nonprime part */
            for (size_t s = N; ; --s) {          /* back‑to‑front to keep the part distinct */
                dp[s] = dp[s] + dp[s - v];
                if (s == v) break;
            }
        }
    }

    seq_t result = dp[N];   /* The desired partition count for n = N */

    /* Clean up */
    free(dp);
    free(is_prime);

    return result;
}
