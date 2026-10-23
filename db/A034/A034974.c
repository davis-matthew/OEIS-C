/*
 * A034974  Number of divisors of binomial(n, floor(n/2)), the terms of
 *          A001405.
 * https://oeis.org/A034974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number_of_divisors( binomial(n, floor(n/2)) )
 * Range:     u8 n <= 26, u16 n <= 74, u32 n <= 177, u64 n <= 452,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache of primes up to the current maximum limit */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_max = 0;

/* Ensure that primes contain all primes ≤ limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max)
        return;

    /* Allocate an array for the sieve. */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (sz > 0) is_prime[0] = false;
    if (sz > 1) is_prime[1] = false;

    /* Classic Eratosthenes sieve. */
    for (idx_t p = 2; (idx_t)(p * p) <= limit; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[(size_t)m] = false;
        }
    }

    /* Count primes and store them. */
    idx_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            new_primes[pos++] = (uint64_t)i;

    free(is_prime);
    free(primes);
    primes = new_primes;
    prime_cnt = (size_t)cnt;
    prime_max = limit;
}

/* Compute the sequence value. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but guard against 0 */
        return (seq_t)0;

    idx_t k = n >> 1;   /* floor(n/2) */

    /* Build the prime table up to n. */
    ensure_primes(n);

    seq_t tau = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n)   /* safe guard */
            break;

        /* Legendre exponents for the central binomial coefficient. */
        idx_t e = 0;
        idx_t t = n;
        while (t) { t /= p; e += t; }
        t = k;
        while (t) { t /= p; e -= t; }
        t = n - k;
        while (t) { t /= p; e -= t; }

        if (e > 0)
            tau *= (seq_t)(e + 1);
    }

    return tau;
}
