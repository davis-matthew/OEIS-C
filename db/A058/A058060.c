/*
 * A058060  Number of distinct prime factors of d(n), the number of divisors
 *          of n.
 * https://oeis.org/A058060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime factors of the divisor function
 *            τ(n). That is a(n) = ω(τ(n))
 * Range:     u8 n <= 5001, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Dynamically compute a list of primes up to LIMIT for factorisations. */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    const size_t LIMIT = 100000;          /* sufficient for n ≤ 10^9 */
    uint8_t *is_prime = (uint8_t *)malloc(LIMIT + 1);
    if (!is_prime)
        return;         /* could not allocate; number theory will fail */
    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_cnt;

    primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = (idx_t)i;

    free(is_prime);
    primes_initialized = 1;
}

static inline seq_t count_distinct_prime_factors(idx_t x)
{
    seq_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > x)
            break;
        if (x % p == 0) {
            ++cnt;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) /* undefined, but guard */
        return 0;

    init_primes();

    /* compute τ(n) */
    idx_t rem = n;
    idx_t tau = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > rem)
            break;
        if (rem % p == 0) {
            idx_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (rem > 1)
        tau *= 2;  /* remaining prime factor */

    /* count distinct prime factors of τ(n) */
    return count_distinct_prime_factors(tau);
}
