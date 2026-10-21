/*
 * A224979  Number of primes of the form p-q+1 where q is any prime < p =
 *          prime(n).
 * https://oeis.org/A224979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form p - q + 1 where p = prime(n)
 *            (OEIS A224979)
 * Range:     u8 n <= 397, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_upper_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_upper_limit.h"

static uint64_t *primes_list = NULL; /* list of primes found so far */
static size_t primes_cnt = 0;        /* number of primes in primes_list   */
static idx_t   max_limit   = 0;      /* maximum number sieved             */
static bool   *is_prime_arr = NULL;  /* sieve results up to max_limit    */

/* Build the prime sieve up to 'limit' and store primes and sieve array. */
static void build_sieve(idx_t limit)
{
    /* allocate sieve array */
    bool *new_is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!new_is_prime) abort();
    for (idx_t i = 2; i <= limit; ++i) new_is_prime[i] = true;

    /* simple Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i)
        if (new_is_prime[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                new_is_prime[j] = false;
        }

    /* count primes */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_is_prime[i]) ++prime_cnt;

    /* copy primes */
    uint64_t *new_primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_is_prime[i]) new_primes[idx++] = (uint64_t)i;

    /* free old structures */
    free(is_prime_arr);
    free(primes_list);

    /* store new data */
    is_prime_arr = new_is_prime;
    primes_list = new_primes;
    primes_cnt = prime_cnt;
    max_limit   = limit;
}

/* Ensure that the sieve contains at least 'n' primes. */
static void ensure_n_primes(idx_t n)
{
    if (primes_cnt >= (size_t)n) return;

    idx_t limit = estimate_upper_limit(n);
    if (limit <= max_limit) limit = max_limit + 100;  /* increase a bit */

    build_sieve(limit);

    /* In the unlikely case the estimate was too small, extend again */
    while (primes_cnt < (size_t)n)
    {
        limit = max_limit + (max_limit / 2) + 50;
        build_sieve(limit);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* not defined for n < 1 */

    /* make sure we have enough primes */
    ensure_n_primes(n);

    uint64_t p = primes_list[n - 1];  /* nth prime (offset 1) */

    seq_t count = 0;
    for (size_t i = 0; i < (size_t)(n - 1); ++i)
    {
        uint64_t q = primes_list[i];
        uint64_t r = p - q + 1;
        if (r <= max_limit && is_prime_arr[r]) ++count;
    }
    return count;
}
