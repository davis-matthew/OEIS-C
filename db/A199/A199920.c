/*
 * A199920  Number of ways to write n = p+k with p, p+6, 6k-1 and 6k+1 all
 *          prime.
 * https://oeis.org/A199920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 9108, u16 n <= 9108, u32 n <= 9108, u64 n <= 9108,
 *            u128 n <= 9108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A199920: a(n) = number of ways to write n = p + k
   with p,p+6,6k-1,6k+1 all prime.  The function works for any
   seq_t and idx_t supplied by the build. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static data for a lazy sieve of Eratosthenes. */
static uint8_t *isPrime = NULL;     /* 0/1 array */
static uint64_t *primes = NULL;     /* list of primes */
static size_t primes_count = 0;    /* number of primes stored */
static idx_t sieve_limit = 0;       /* highest number currently sieved */
static bool sieve_initialized = false;

/* Initialise (or extend) the sieve up to at least 'limit'. */
static void init_sieve(idx_t limit)
{
    /* already sufficient? */
    if (sieve_initialized && limit <= sieve_limit)
        return;

    /* free old data */
    free(isPrime);
    free(primes);
    isPrime = NULL;
    primes = NULL;
    primes_count = 0;
    sieve_initialized = false;

    /* allocate and mark all true initially */
    isPrime = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!isPrime) exit(1);                 /* out of memory */
    memset(isPrime, 1, (size_t)(limit + 1));
    if (limit >= 0) isPrime[0] = 0;
    if (limit >= 1) isPrime[1] = 0;

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isPrime[j] = 0;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) ++cnt;

    /* store primes in an array for fast iteration */
    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) exit(1);
    primes_count = cnt;
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) primes[pos++] = (uint64_t)i;

    sieve_limit = limit;
    sieve_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)               /* below offset */
        return 0;

    /* The maximal number we need to test for primality is
       max(p+6, 6k-1, 6k+1) ≤ 6*n+1.  Use that as sieve limit. */
    idx_t limit = 6 * n + 1;
    init_sieve(limit);

    seq_t count = 0;

    /* Iterate over all primes p < n */
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p >= (uint64_t)n)
            break;           /* p < n required */

        idx_t k = n - (idx_t)p;            /* k ≥ 1 */

        /* candidate numbers to test for primality */
        idx_t p6   = (idx_t)p + 6;
        idx_t k6m1 = 6 * k - 1;
        idx_t k6p1 = 6 * k + 1;

        /* check all four primes */
        if (isPrime[p6] && isPrime[k6m1] && isPrime[k6p1]) {
            ++count;
        }
    }
    return count;
}
