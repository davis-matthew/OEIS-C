/*
 * A102108  Numbers of partitions of 2n into n primes.
 * https://oeis.org/A102108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 2*n into prime parts
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 0 is not part of the defined sequence, but safe to return 0. */
    if (n == 0) return (seq_t)0;

    /* Target sum = 2*n. */
    idx_t target = n * (idx_t)2;

    /* Static state for memoisation. */
    static idx_t current_max_sum = 0;   /* largest sum processed so far */
    static seq_t *dp = NULL;            /* DP array: dp[s] = partitions of s */
    static idx_t *primes = NULL;        /* primes up to current_max_sum */
    static idx_t primes_count = 0;      /* number of primes stored */

    /* Recompute if a larger sum is requested. */
    if (target > current_max_sum)
    {
        /* Free previous arrays if they exist. */
        if (dp) free(dp);
        if (primes) free(primes);

        /* Allocate DP array for the new range, initialise to 0. */
        dp = (seq_t *)calloc((size_t)target + 1, sizeof(seq_t));
        dp[0] = (seq_t)1;   /* one way to partition 0 */

        /* Sieve of Eratosthenes up to target. */
        bool *is_prime = (bool *)calloc((size_t)target + 1, sizeof(bool));
        for (idx_t i = 2; i <= target; ++i) is_prime[i] = true;
        for (idx_t i = 2; i * i <= target; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= target; j += i)
                    is_prime[j] = false;

        /* Count primes to allocate array. */
        primes_count = 0;
        for (idx_t i = 2; i <= target; ++i)
            if (is_prime[i]) ++primes_count;
        primes = (idx_t *)malloc((size_t)primes_count * sizeof(idx_t));

        /* Store primes. */
        idx_t pc = 0;
        for (idx_t i = 2; i <= target; ++i)
            if (is_prime[i]) primes[pc++] = i;

        free(is_prime);   /* sieve temporary */

        /* Unbounded knapsack DP: count partitions into primes. */
        for (idx_t pi = 0; pi < primes_count; ++pi)
        {
            idx_t p = primes[pi];
            for (idx_t s = p; s <= target; ++s)
                dp[s] += dp[s - p];
        }

        current_max_sum = target;
    }

    return dp[(size_t)target];
}
