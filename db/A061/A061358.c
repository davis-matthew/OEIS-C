/*
 * A061358  Number of ways of writing n = p+q with p, q primes and p >= q.
 * https://oeis.org/A061358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered partitions of n into two primes.
 *            (OEIS A061358)
 * Range:     u8 n <= 6929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for prime sieve and prime list */
static uint8_t *is_prime = NULL;       /* 0/1 flags for primality */
static idx_t prime_limit = 0;          /* highest number sieved */
static idx_t *primes = NULL;           /* list of primes up to prime_limit */
static size_t prime_count = 0;         /* number of primes stored */

/* rebuild the sieve and prime list up to the given limit */
static void build_primes(idx_t limit)
{
    /* free old tables if any */
    free(is_prime);
    free(primes);

    /* allocate primality table */
    is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return;                     /* allocation failure; caller will see 0 count */

    /* initialise all numbers as prime; handle 0 and 1 separately */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0)
        is_prime[0] = 0;
    if (limit >= 1)
        is_prime[1] = 0;

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = 0;
        }
    }

    /* count primes to size the array */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_count;

    /* allocate prime list */
    primes = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!primes)
        return;                     /* allocation failure */

    /* fill the prime list */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
        }
    }

    prime_limit = limit;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* a(0)=a(1)=0 */

    /* ensure we have primes up to n */
    if (prime_limit < n)
        build_primes(n);

    /* count unordered prime pairs p>=q summing to n */
    seq_t count = 0;
    idx_t half = n / 2;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p > half)
            break;
        if (is_prime[n - p])
            ++count;
    }
    return count;
}
