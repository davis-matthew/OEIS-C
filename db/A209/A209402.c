/*
 * A209402  Number of partitions of n into distinct primes except the prime
 *          factors of n.
 * https://oeis.org/A209402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts partitions of n into distinct primes NOT dividing n.
 *            This implementation uses a simple sieve to generate the primes ≤
 *            n, then a classic knapsack DP to count the partitions. The
 *            algorithm runs in O(n · π(n)) time, which is well below 2 s for
 *            n up to 10 000
 * Range:     u8 n <= 106, u16 n <= 310, u32 n <= 1038, u64 n <= 3876,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 (empty partition). */
    if (n == 0)
        return (seq_t)1;

    /* ---------- 1. Generate all primes ≤ n (simple sieve) ---------- */
    /* Allocate boolean array for the sieve. */
    idx_t *is_composite = (idx_t *)calloc((size_t)(n + 1), sizeof(idx_t));
    if (is_composite == NULL)
        return (seq_t)0;   /* out of memory – return 0 as a safe fallback */

    /* Count primes and collect them into a dynamic array. */
    size_t prime_cnt = 0;
    /* Allocate array of size n/2 for prime storage – enough for n≤10⁴. */
    uint64_t *primes = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (primes == NULL) {
        free(is_composite);
        return (seq_t)0;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            /* i is prime */
            primes[prime_cnt++] = (uint64_t)i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);   /* sieve array no longer needed */

    /* ---------- 2. DP: count partitions using allowed primes ---------- */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL) {
        free(primes);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;   /* one way: use no primes */

    /* Process each prime that does NOT divide n. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (n % p == 0)
            continue;   /* skip prime factors of n */

        /* Update dp from high to low to enforce distinctness. */
        for (idx_t s = n; s >= p; --s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[n];

    /* ---------- 3. Clean up and return ---------- */
    free(dp);
    free(primes);
    return result;
}
