/*
 * A105117  Minimal number of perfect powers needed to sum to prime(n).
 * https://oeis.org/A105117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Minimal number of perfect powers needed to sum to prime(n)
 *  a(n) = minimum k such that prime(n) = p1 + … + pk,
 *  each pi is a perfect power (i.e. m^e with m ≥ 1, e ≥ 2).
 *  The implementation precomputes primes via a simple sieve and
 *  uses an unlimited‑coin dynamic‑programming table of minimal
 *  summands for all integers up to each prime.
 *
 *  The algorithm is O(p(n)·π(p(n))) to build the DP table for the
 *  nth prime and runs in well under 2 s for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"  /* estimate_prime_upper_bound */

static idx_t    primes_cnt      = 0;         /* number of primes computed   */
static idx_t   *primes_arr      = NULL;      /* array of primes             */
static idx_t   *prime_sieve     = NULL;      /* temporary sieve array       */
static idx_t   prime_sieve_len  = 0;         /* size of sieve array         */

static uint16_t *dp_arr          = NULL;     /* DP array for minimal coins  */
static idx_t    dp_arr_len       = 0;         /* size of DP array            */

static void ensure_prime_sieve(idx_t bound)
{
    if (bound < prime_sieve_len) return;
    free(prime_sieve);
    prime_sieve_len = bound + 1;
    prime_sieve = (idx_t*)calloc(prime_sieve_len, sizeof(idx_t));
    if (!prime_sieve) { abort(); }
    for (idx_t i = 2; i <= bound; ++i) prime_sieve[i] = 1;
    for (idx_t i = 2; i * i <= bound; ++i) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= bound; j += i) prime_sieve[j] = 0;
        }
    }
    free(primes_arr);
    primes_cnt = 0;
    for (idx_t i = 2; i <= bound; ++i) if (prime_sieve[i]) primes_cnt++;
    primes_arr = (idx_t*)malloc(primes_cnt * sizeof(idx_t));
    if (!primes_arr) { abort(); }
    idx_t idx = 0;
    for (idx_t i = 2; i <= bound; ++i) if (prime_sieve[i]) primes_arr[idx++] = i;
}

static void ensure_dp(idx_t limit)
{
    if (dp_arr_len > limit) return;           /* already computed up to 'limit' */
    if (dp_arr) free(dp_arr);
    dp_arr_len = limit + 1;
    dp_arr = (uint16_t*)malloc(dp_arr_len * sizeof(uint16_t));
    if (!dp_arr) { abort(); }
    const uint16_t INF = 0xFFFF;
    for (idx_t i = 0; i <= limit; ++i) dp_arr[i] = INF;
    dp_arr[0] = 0;
    /* Generate perfect powers ≤ limit (including 1) */
    idx_t coins_sz = 0;
    idx_t coins_cap = 64;
    idx_t *coins = (idx_t*)malloc(coins_cap * sizeof(idx_t));
    if (!coins) { abort(); }
    coins[coins_sz++] = 1;                     /* 1^k for any k */
    for (idx_t base = 2; base * base <= limit; ++base) {
        unsigned __int128 val = (unsigned __int128)base * base;
        while (val <= limit) {
            if (coins_sz >= coins_cap) {
                coins_cap *= 2;
                coins = (idx_t*)realloc(coins, coins_cap * sizeof(idx_t));
                if (!coins) { abort(); }
            }
            coins[coins_sz++] = (idx_t)val;
            val *= base;
        }
    }
    /* Unlimited‑coin DP (classic coin change) */
    for (idx_t ci = 0; ci < coins_sz; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t s = coin; s <= limit; ++s) {
            uint16_t prev = dp_arr[s - coin];
            if (prev != INF) {
                uint16_t cand = prev + 1;
                if (cand < dp_arr[s]) dp_arr[s] = cand;
            }
        }
    }
    free(coins);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range; not used by OEIS */
    /* Estimate an upper bound for the n-th prime */
    idx_t bound = estimate_prime_upper_bound(n) + 10;
    ensure_prime_sieve(bound);
    if (n > primes_cnt) {
        bound = estimate_prime_upper_bound(n * 2) + 10;
        ensure_prime_sieve(bound);
    }
    idx_t prime_n = primes_arr[n - 1];
    ensure_dp(prime_n);
    return (seq_t)dp_arr[prime_n];
}
