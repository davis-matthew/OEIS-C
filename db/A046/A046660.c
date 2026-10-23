/*
 * A046660  Excess of n = number of prime divisors (with multiplicity) -
 *          number of prime divisors (without multiplicity).
 * https://oeis.org/A046660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Omega(n) - omega(n) (Omega = total prime factors with
 *            multiplicity, omega = number of distinct prime factors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root using binary search (returns ⌊√n⌋). */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq = (idx_t)((uint64_t)mid * (uint64_t)mid);
        if (sq == n) return mid;
        if (sq < n)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* Cached list of primes up to the largest limit seen. */
static uint64_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0;

/* Simple sieve that extends the cache up to LIMIT (inclusive). */
static void sieve_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;   /* already have enough */

    /* Allocate a temporary bit array for the sieve. */
    bool *is_prime = (bool*)malloc((size_t)limit + 1);
    if (!is_prime) return;              /* memory failure – keep old cache */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count new primes and enlarge the cache. */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_cnt;

    uint64_t *new_cache = (uint64_t*)realloc(prime_cache,
                                            sizeof(uint64_t) * new_cnt);
    if (!new_cache) {
        free(is_prime);
        return;                       /* keep old cache if realloc fails */
    }
    prime_cache = new_cache;
    prime_cnt   = new_cnt;
    prime_limit = limit;

    /* Store prime numbers in the cache. */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_cache[idx++] = (uint64_t)i;

    free(is_prime);
}

/* Sequence A046660: excess of prime factors (with multiplicity minus
   distinct). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n = 0 */

    /* Ensure we have all primes needed for factorisation. */
    idx_t limit = isqrt_idx((idx_t)n) + 1;
    sieve_primes_upto(limit);

    uint64_t tmp = (uint64_t)n;
    seq_t    Omega = 0;  /* total number of prime factors (with multiplicity) */
    seq_t    omega = 0;  /* number of distinct prime factors */

    for (size_t i = 0; i < prime_cnt &&
            (uint64_t)prime_cache[i] * prime_cache[i] <= tmp; ++i)
    {
        uint64_t p = prime_cache[i];
        if (tmp % p != 0) continue;

        seq_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        Omega += exp;
        ++omega;
    }

    /* A remaining factor > sqrt(n) is a prime itself. */
    if (tmp > 1) {
        ++Omega;
        ++omega;
    }

    return Omega - omega;
}
