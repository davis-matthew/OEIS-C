/*
 * A054685  Number of partitions of n into distinct prime powers (1 not
 *          considered a power).
 * https://oeis.org/A054685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct prime powers (1
 *            is not considered a power)
 * Range:     u8 n <= 53, u16 n <= 168, u32 n <= 636, u64 n <= 2680,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Memoisation storage for the dynamic‑programming table. */
static seq_t *dp = NULL;      /* dp[i] holds a(i) for 0 <= i < dp_size */
static idx_t  dp_size = 0;    /* current maximum index held (exclusive) */

/* Recompute the table dp[0 .. limit] from scratch. */
static void recompute_dp(idx_t limit)
{
    /* Allocate a new table and set the base case a(0)=1. */
    seq_t *new_dp = (seq_t*)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!new_dp) exit(1);
    new_dp[0] = (seq_t)1;               /* a(0) = 1 */

    /* Sieve of Eratosthenes to obtain all primes up to limit. */
    size_t m = (size_t)limit;
    bool *is_prime = (bool*)calloc(m + 1, sizeof(bool));
    if (!is_prime) { free(new_dp); exit(1); }
    for (size_t i = 0; i <= m; ++i) is_prime[i] = true;
    if (m >= 0) is_prime[0] = false;
    if (m >= 1) is_prime[1] = false;
    for (size_t p = 2; p * p <= m; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= m; q += p)
                is_prime[q] = false;

    /* Dynamic‑programming over each distinct prime power ≤ limit. */
    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_prime[(size_t)p]) continue;   /* skip non‑primes */
        unsigned long long val = (unsigned long long)p;   /* p^1 */
        while (val <= (unsigned long long)limit)
        {
            /* Update dp in reverse to avoid reuse of the same power. */
            for (unsigned long long t = (unsigned long long)limit; t >= val; --t)
            {
                new_dp[t] += new_dp[t - val];
                if (t == val) break;          /* avoid underflow */
            }
            if (val > (unsigned long long)limit / (unsigned long long)p) break;
            val *= (unsigned long long)p;       /* next power of p */
        }
    }

    /* Replace the old table. */
    free(is_prime);
    free(dp);
    dp       = new_dp;
    dp_size  = limit + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (dp == NULL || n >= dp_size)
        recompute_dp(n);
    return dp[n];
}
