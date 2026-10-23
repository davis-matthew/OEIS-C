/*
 * A159802  Number of primes q with (2m)^2+1 <= q < (2m+1)^2-2m.
 * https://oeis.org/A159802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q with (2n)^2+1 ≤ q < (2n+1)^2-2n (i.e.
 *            count of primes in the interval [4n²+1 , 4n²+2n+1) )
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static cache of base primes up to the current limit */
static idx_t prime_limit = 0;          /* highest integer up to which primes are known */
static uint32_t *primes = NULL;        /* array of primes */
static size_t primes_len = 0;          /* number of primes stored */

/* simple integer square‑root (floor) for idx_t */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t r = 1;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Extend the prime list so that all primes ≤ limit are available */
static void extend_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Allocate a simple sieve up to 'limit' */
    size_t sz = (size_t)(limit + 1);
    char *is_prime = (char *)malloc(sz);
    if (!is_prime)
        exit(1);
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate the new array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_primes)
        exit(1);
    primes = new_primes;
    primes_len = cnt;

    /* Store primes */
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            primes[k++] = (uint32_t)i;
    }

    free(is_prime);
    prime_limit = limit;
}

/* Count primes in [low, high) using the cached primes */
static seq_t count_primes_in_range(idx_t low, idx_t high)
{
    if (low >= high)
        return (seq_t)0;   /* empty range */

    idx_t length = high - low;            /* number of numbers in interval */
    char *segment = (char *)calloc((size_t)length, 1);
    if (!segment)
        exit(1);

    /* Ensure we have all primes up to sqrt(high-1) */
    idx_t max_candidate = high - 1;
    idx_t sqrt_limit = int_sqrt(max_candidate) + 1;
    extend_prime_sieve(sqrt_limit);

    /* Segmented sieve */
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > max_candidate)
            break;

        /* first multiple of p within [low, high-1] */
        idx_t start = (low + p - 1) / p * p;
        if (start < p * p)
            start = p * p;

        for (idx_t j = start; j <= max_candidate; j += p) {
            segment[j - low] = 1;
        }
    }

    /* Count primes */
    seq_t count = 0;
    for (size_t i = 0; i < (size_t)length; ++i) {
        if (!segment[i])
            ++count;
    }

    free(segment);
    return count;
}

/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for the offset 1 sequence, return 0 */

    /* Compute bounds of the interval */
    idx_t L = 4 * n * n + 1;                   /* inclusive lower bound */
    idx_t U = 4 * n * n + 2 * n + 1;           /* exclusive upper bound */

    return count_primes_in_range(L, U);
}
