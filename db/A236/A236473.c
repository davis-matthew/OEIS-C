/*
 * A236473  Number of partitions into multiplicatively perfect numbers, cf.
 *          A007422.
 * https://oeis.org/A236473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 121, u32 n <= 364, u64 n <= 1217,
 *            u128 n <= 4395, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions into multiplicatively perfect numbers (and 1).
   A multiplicatively perfect number has exactly four divisors (numbers of the
   form p³ or p⋅q with distinct primes p,q).  The function counts the
   number of unordered additive partitions of n whose parts are either 1 or
   such multiplicatively perfect numbers.  The result is computed by a
   classic unbounded coin‑change dynamic program and memoised for
   subsequent calls.  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoised DP table and the largest n that has been computed. */
    static seq_t *dp = NULL;
    static idx_t  max_n = 0;

    /* If the value is already available, return it. */
    if (dp && n <= max_n) {
        return dp[n];
    }

    /* Allocate a new DP table for 0 … n. */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_dp = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_dp) exit(EXIT_FAILURE);
    new_dp[0] = 1; /* empty partition */

    /* ------------------------------------------------------------------
       1. Sieve to collect all primes up to n. */
    bool *isprime = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!isprime) { free(new_dp); exit(EXIT_FAILURE); }
    for (size_t i = 2; i <= n; ++i) isprime[i] = true;
    for (size_t i = 2; (size_t)i * i <= n; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= n; j += i) isprime[j] = false;
        }
    }
    /* Store primes in a dynamic array. */
    idx_t *primes = NULL;
    size_t primes_cnt = 0, primes_cap = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (isprime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? 2 * primes_cap : 4;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_cnt++] = (idx_t)i;
        }
    }
    free(isprime);

    /* ------------------------------------------------------------------
       2. Build the list of coins: 1 together with all numbers with four
          divisors (p³ and p⋅q). */
    idx_t *coins = NULL;
    size_t coins_cnt = 0, coins_cap = 0;

    /* Include 1. */
    coins_cap = 4; coins = (idx_t *)malloc(coins_cap * sizeof(idx_t));
    coins[coins_cnt++] = 1;

    /* Prime cubes. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        idx_t cube = p * p * p;
        if (cube <= n) {
            if (coins_cnt == coins_cap) {
                coins_cap *= 2;
                coins = (idx_t *)realloc(coins, coins_cap * sizeof(idx_t));
            }
            coins[coins_cnt++] = cube;
        }
    }

    /* Products of two distinct primes. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        for (size_t j = i + 1; j < primes_cnt; ++j) {
            idx_t q = primes[j];
            idx_t prod = p * q;
            if (prod > n) break;
            if (coins_cnt == coins_cap) {
                coins_cap *= 2;
                coins = (idx_t *)realloc(coins, coins_cap * sizeof(idx_t));
            }
            coins[coins_cnt++] = prod;
        }
    }
    free(primes);

    /* ------------------------------------------------------------------
       3. Unbounded coin change DP. */
    for (size_t ci = 0; ci < coins_cnt; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i) {
            new_dp[i] += new_dp[i - c];
        }
    }
    free(coins);

    /* Replace the old table with the new one. */
    if (dp) free(dp);
    dp = new_dp;
    max_n = n;

    return dp[n];
}
