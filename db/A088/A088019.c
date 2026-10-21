/*
 * A088019  Number of twin primes between n and 2n (inclusive).
 * https://oeis.org/A088019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ p prime : n ≤ p ≤ 2n and (p-2 is prime or p+2 is
 *            prime) }
 * Range:     u8 n <= 9066, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a prime sieve up to 2n+2 (only when a larger
   limit is required) and counts all primes inside [n,2n] that belong to a
   twin‑prime pair.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration */
static void ensure_sieve(idx_t limit);

/* Static storage for the prime sieve and list of primes.  */
static uint8_t *isPrime   = NULL; /* 1 = prime, 0 = composite */
static idx_t   *primes    = NULL; /* array of prime numbers */
static idx_t    sieveLimit = 0;   /* currently sieved maximum index */
static size_t   primeCount = 0;   /* number of primes stored */

/* Build or extend the sieve up to 'limit' (inclusive) */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieveLimit)          /* already large enough */
        return;

    /* Allocate array for primality flags */
    size_t sz = (size_t)(limit + 1);
    uint8_t *newIsPrime = (uint8_t *)malloc(sz);
    if (!newIsPrime)
        exit(EXIT_FAILURE);          /* allocation failed */

    /* Initialise to true (prime) except 0 and 1 */
    for (size_t i = 0; i < sz; ++i)
        newIsPrime[i] = 1;
    newIsPrime[0] = newIsPrime[1] = 0;

    /* Classic Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (newIsPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                newIsPrime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (newIsPrime[i]) ++cnt;

    /* Allocate array of prime numbers */
    idx_t *newPrimes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!newPrimes)
        exit(EXIT_FAILURE);          /* allocation failed */

    /* Fill prime array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (newIsPrime[i]) newPrimes[idx++] = i;

    /* Free old storage */
    free(isPrime);
    free(primes);

    /* Install new storage */
    isPrime   = newIsPrime;
    primes    = newPrimes;
    sieveLimit = limit;
    primeCount = cnt;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return 0;

    idx_t two_n = 2 * n;
    idx_t limit = two_n + 2;            /* need primes up to 2n+2 */
    ensure_sieve(limit);

    seq_t count = 0;

    for (size_t i = 0; i < primeCount; ++i) {
        idx_t p = primes[i];
        if (p < n)                 /* below interval */
            continue;
        if (p > two_n)             /* beyond interval */
            break;

        bool is_twin = false;

        if (p > 2 && isPrime[p - 2])
            is_twin = true;

        if (!is_twin && isPrime[p + 2])
            is_twin = true;

        if (is_twin)
            ++count;
    }

    return count;
}
