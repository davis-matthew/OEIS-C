/*
 * A100950  Number of partitions of n into a coprime pair of a prime and a
 *          semiprime.
 * https://oeis.org/A100950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,s) with p prime, s semiprime, p + s =
 *            n, and gcd(p,s) = 1
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for the required types and functions */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t   max_limit = 0;      /* largest n for which data is built        */
static bool   *is_prime_arr   = NULL;   /* is_prime_arr[i] = true iff i is prime  */
static bool   *is_semiprime_arr = NULL; /* is_semiprime_arr[i] = true iff i is semiprime */
static uint64_t *primes       = NULL;   /* list of primes up to max_limit          */
static size_t   prime_count   = 0;      /* number of primes in primes[]            */

/* Builds prime and semiprime tables up to at least n. */
static void init_primes(idx_t n)
{
    /* No work if table is already up to date. */
    if (n <= max_limit) return;

    /* Allocate new arrays of size n+1 (index 0..n). */
    size_t size = (size_t)n + 1;
    bool *new_is_prime = (bool *)calloc(size, sizeof(bool));
    bool *new_is_semip  = (bool *)calloc(size, sizeof(bool));
    if (!new_is_prime || !new_is_semip) abort();

    /* Sieve of Eratosthenes. */
    for (size_t i = 2; i < size; ++i) new_is_prime[i] = true;
    for (size_t i = 2; i * i <= n; ++i)
        if (new_is_prime[i])
            for (size_t j = i * i; j <= n; j += i)
                new_is_prime[j] = false;

    /* Count primes and allocate a list. */
    size_t cnt = 0;
    for (size_t i = 2; i < size; ++i) if (new_is_prime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    size_t k = 0;
    for (size_t i = 2; i < size; ++i)
        if (new_is_prime[i]) new_primes[k++] = i;

    /* Mark semiprimes: products of two primes (allowing repetition). */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = new_primes[i];
        for (size_t j = i; j < cnt; ++j) {
            uint64_t q = new_primes[j];
            if (q > n / p) break;                 /* q*p > n */
            uint64_t prod = p * q;                /* <= n */
            if (prod > n) break;
            new_is_semip[(size_t)prod] = true;
        }
    }

    /* Replace old tables with the new ones. */
    free(is_prime_arr);
    free(is_semiprime_arr);
    free(primes);
    is_prime_arr = new_is_prime;
    is_semiprime_arr = new_is_semip;
    primes = new_primes;
    prime_count = cnt;
    max_limit = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* smallest possible sum 2 (prime) + 4 (semiprime) */

    init_primes(n);               /* ensure tables up to n are available */

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > n - 4) break;     /* semiprime minimal value is 4 */
        uint64_t s = n - p;       /* remaining part */
        if (!is_semiprime_arr[(size_t)s]) continue;
        if (s % p == 0) continue; /* gcd(p,s) > 1 */
        ++count;
    }
    return count;
}
