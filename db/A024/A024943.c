/*
 * A024943  Number of partitions of n into distinct 6k+1 primes.
 * https://oeis.org/A024943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct primes of the
 *            form 6k+1. Computation: 0‑1 knapsack over all primes p ≤ n with
 *            p % 6 == 1
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Simple sieve of Eratosthenes: returns array of primes ≤ limit. */
static void _sieve_primes_upto(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) { *primes_out = NULL; *cnt_out = 0; return; }

    /* Use a byte array for the sieve flags. */
    size_t len = (size_t)limit + 1;
    uint8_t *sieve = (uint8_t *)malloc(len);
    if (!sieve) { *primes_out = NULL; *cnt_out = 0; return; }

    for (size_t i = 0; i < len; ++i) sieve[i] = 1;   /* mark all as prime initially */
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (sieve[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes to allocate output array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    idx_t *primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) { free(sieve); *primes_out = NULL; *cnt_out = 0; return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes[idx++] = (idx_t)i;

    free(sieve);
    *primes_out = primes;
    *cnt_out = cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Generate all primes up to n. */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    _sieve_primes_upto(n, &primes, &prime_cnt);
    if (!primes) return 0;          /* allocation failure */

    /* Allocate DP array: dp[i] = number of partitions of i. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) { free(primes); return 0; }

    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;

    /* 0‑1 knapsack over primes ≡ 1 (mod 6). */
    for (size_t pi = 0; pi < prime_cnt; ++pi)
    {
        idx_t p = primes[pi];
        if (p % 6 != 1) continue;

        for (idx_t j = n; j >= p; --j)
            dp[j] += dp[j - p];
    }

    seq_t result = dp[n];

    free(primes);
    free(dp);
    return result;
}
