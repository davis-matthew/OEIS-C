/*
 * A097277  a(n) = number of permutations <p(1), p(2), ..., p(n)> of <1, 2,
 *          ..., n>, such that p(k) > p(k-1) when k is composite and p(k) <
 *          p(k-1) when k is prime.
 * https://oeis.org/A097277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of permutations of 1..n such that
 * p(k) > p(k-1) when k is composite,
 * p(k) < p(k-1) when k is prime (k >= 2).
 * Computed by dynamic programming over the prime/composite pattern. */

#include <stdlib.h>
#include <stdbool.h>

/* Return a(n) for n >= 1 (a(0) is defined as 0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* 1.  Sieve primality for indices up to n */
    bool *isPrime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!isPrime) return (seq_t)0;  /* allocation failure */
    for (idx_t i = 0; i <= n; ++i) isPrime[i] = true;
    isPrime[0] = false;  isPrime[1] = false;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (isPrime[p]) {
            for (idx_t j = p * p; j <= n; j += p) isPrime[j] = false;
        }
    }

    /* 2.  DP arrays: dp[r] = #permutations of current length ending
     *     with rank r (1‑based among the current set of numbers). */
    seq_t *dp     = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    seq_t *new_dp = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    if (!dp || !new_dp) {
        free(isPrime);
        free(dp); free(new_dp);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n + 1; ++i) dp[i] = 0;
    dp[1] = (seq_t)1;          /* base case: length 1, rank 1 */

    /* 3.  Iterate over permutation lengths */
    for (idx_t len = 2; len <= n; ++len) {
        idx_t old_len = len - 1;  /* previous length */

        if (isPrime[len]) {      /* pattern '<' : new element must be smaller */
            /* new_dp[j] = sum_{i>=j} dp[i]  for j = 1..old_len */
            seq_t suffix = 0;
            for (idx_t j = old_len; j > 0; --j) {
                suffix = suffix + dp[j];
                new_dp[j] = suffix;
            }
            new_dp[len] = 0;     /* no rank len possible after a down step */
        } else {                 /* pattern '>' : new element must be larger */
            new_dp[1] = 0;       /* rank 1 impossible after an up step */
            seq_t prefix = 0;
            for (idx_t j = 1; j <= old_len; ++j) {
                prefix = prefix + dp[j];
                new_dp[j + 1] = prefix;
            }
            /* new_dp[1] remains 0; new_dp[len] computed by prefix loop when j=old_len */
        }

        /* swap pointers to reuse the two arrays */
        seq_t *tmp = dp; dp = new_dp; new_dp = tmp;
    }

    /* 4.  Sum counts of all possible last ranks to obtain a(n) */
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) total = total + dp[i];

    free(isPrime);
    free(dp);  free(new_dp);   /* both pointers are no longer needed */
    return total;
}
