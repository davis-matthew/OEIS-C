/*
 * A237720  Number of primes p <= (n+1)/2 with floor( sqrt(n-p) ) prime.
 * https://oeis.org/A237720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime p <= (n+1)/2 such that floor(sqrt(n-p))
 *            is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Basic integer square root (floor of sqrt).  Suitable for n <= 10^8. */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t low = 0, high = x, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            r   = mid;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return r;
}

/* Trial‑division primality test for small numbers (<= 10^8). */
static inline bool simple_is_prime(idx_t v)
{
    if (v < 2)
        return false;
    if (v % 2ULL == 0ULL)
        return v == 2U;
    for (idx_t d = 3ULL; d * d <= v; d += 2ULL)
        if (v % d == 0ULL)
            return false;
    return true;
}

/* Prime table that can grow up to the largest requested limit. */
static uint64_t *prime_table   = NULL;
static size_t   prime_count    = 0;
static idx_t    prime_max_limit = 0;

/* Ensure that prime_table contains all primes <= limit. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_max_limit && prime_table != NULL)
        return;

    /* Free previous table (if any). */
    if (prime_table != NULL)
        free(prime_table);

    /* Allocate simple sieve array. */
    size_t sieve_size = (size_t) limit + 1;
    bool *sieve = (bool *)malloc(sieve_size);
    if (!sieve)
        return; /* Allocation failed – in practice never happens for small limits. */

    for (size_t i = 0; i < sieve_size; ++i)
        sieve[i] = true;
    if (sieve_size > 0) sieve[0] = false;
    if (sieve_size > 1) sieve[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Allocate and fill the prime table. */
    prime_table = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    for (size_t i = 0, idx = 0; i < cnt; ++i)
        /* nothing */ ;
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i])
            prime_table[idx++] = (uint64_t)i;
    }

    prime_count    = cnt;
    prime_max_limit = limit;
    free(sieve);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, but harmless */

    idx_t limit = (n + 1) / 2;
    ensure_primes_up_to(limit);

    size_t result = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_table[i];
        if (p > limit)
            break;

        idx_t m = n - p;
        idx_t s = int_sqrt(m);
        if (simple_is_prime(s))
            ++result;
    }

    return (seq_t)result;
}
