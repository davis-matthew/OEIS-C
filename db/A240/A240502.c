/*
 * A240502  Product of primes appearing in the factorization of n! with even
 *          exponents.
 * https://oeis.org/A240502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes dividing n! with even exponent
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 77, u64 n <= 151,
 *            u128 n <= 297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached prime data */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Cached primality sieve */
static void *is_prime_arr = NULL;
static idx_t is_prime_limit = 0;

/* Ensure that all primes up to n are sieved and stored. */
static void
ensure_primes(idx_t n)
{
    if (n <= primes_limit)
        return;

    /* Free previous allocations */
    free(is_prime_arr);
    free(primes);

    /* Allocate sieve array */
    is_prime_arr = (void *)malloc((n + 1) * sizeof(char));
    if (!is_prime_arr)
        return;            /* allocation failed – unlikely for small n */

    char *prime_flags = (char *)is_prime_arr;
    for (idx_t i = 0; i <= n; ++i)
        prime_flags[i] = 1;
    if (n >= 0) prime_flags[0] = 0;
    if (n >= 1) prime_flags[1] = 0;

    for (idx_t i = 2; i * i <= n; ++i)
        if (prime_flags[i])
            for (idx_t j = i * i; j <= n; j += i)
                prime_flags[j] = 0;

    /* Count primes and allocate array */
    primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flags[i])
            ++primes_cnt;

    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes)
        return;            /* allocation failed */

    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (prime_flags[i])
            primes[idx++] = i;

    primes_limit = n;
    is_prime_limit = n;
}

/* Compute a(n) for the sequence A240502. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Make sure we have primes up to n. */
    ensure_primes(n);

    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = 0;
        uint64_t pp = p;
        while (pp <= n) {
            exp += n / pp;
            if (pp > n / p)
                break;
            pp *= p;
        }
        if ((exp & 1ULL) == 0) {          /* exponent is even */
            result = result * (seq_t)p;   /* multiply in seq_t type */
        }
    }

    return result;
}
