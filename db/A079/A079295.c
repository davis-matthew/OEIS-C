/*
 * A079295  (D(p)-6)/(12p) where D(p) denotes the denominator of the 2p-th
 *          Bernoulli number and p runs through the primes.
 * https://oeis.org/A079295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079295: a(n) = 1 if the nth prime p satisfies that 2p+1 is prime (i.e. p is a Sophie Germain prime), otherwise 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Static cache of primes generated so far. */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_sz = 0;
static idx_t    prime_cache_limit = 0;

/* Generate all primes up to LIMIT (inclusive).  All work is done in the caller. */
static void generate_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Mark composites. */
    size_t bufsz = (size_t)(limit + 1);
    uint8_t *is_comp = (uint8_t *)malloc(bufsz);
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    memset(is_comp, 0, bufsz);

    idx_t sqrtlim = (idx_t)floor(sqrt((double)limit));
    for (idx_t i = 2; i <= sqrtlim; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    /* Allocate and fill primes array. */
    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_comp);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[idx++] = i;
        }
    }

    free(is_comp);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* Ensure we have at least N primes stored in prime_cache. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_sz >= (size_t)n) return;         /* already enough */

    /* Estimate upper bound for the nth prime (for n >= 6). */
    idx_t estimate;
    if (n < 6) {
        estimate = 15;                               /* primes up to 11 cover n < 6 */
    } else {
        double nd = (double)n;
        estimate = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
    }

    /* Make sure the new limit is larger than the previous one. */
    if (estimate <= prime_cache_limit) {
        estimate = prime_cache_limit * 2 + 100;
    }

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    generate_primes(estimate, &new_primes, &new_cnt);

    free(prime_cache);
    prime_cache = new_primes;
    prime_cache_sz = new_cnt;
    prime_cache_limit = estimate;
}

/* Test if a number is prime by trial division using the primes in prime_cache. */
static bool is_prime_candidate(idx_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < prime_cache_sz; ++i) {
        idx_t p = prime_cache[i];
        if ((idx_t)p * (idx_t)p > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0; return 0 for safety */

    ensure_primes(n);

    idx_t p = prime_cache[n - 1];                /* nth prime */
    idx_t candidate = 2 * p + 1;                /* 2p + 1 */

    bool sg = is_prime_candidate(candidate);
    return (seq_t)(sg ? 1 : 0);
}
