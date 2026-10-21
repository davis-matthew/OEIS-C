/*
 * A092996  Least k such that 1 < p < n < c < k, where p is a prime and c is a
 *          composite number such that for every p there exists a distinct c.
 * https://oeis.org/A092996
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that from n+1 to k-1 there are as many
 *            composite numbers as there are primes less than n. The function
 *            uses a dynamic sieve to generate primes up to required limits
 *            and then iterates composites until the count matches the prime
 *            count
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static global state for the sieve and prime list */
static uint8_t *sieve = NULL;          /* 0 = prime, 1 = composite */
static uint32_t *primes = NULL;        /* list of primes */
static size_t prime_count = 0;         /* number of primes stored */
static size_t prime_cap = 0;           /* capacity of primes array */
static uint32_t max_sieved = 0;         /* highest number sieved */

/* Rebuild the sieve up to 'limit' (inclusive) and recompute the prime list */
static void ensure_sieve_upto(uint32_t limit)
{
    if (limit <= max_sieved)
        return;

    /* Resize sieve array */
    uint8_t *new_sieve = (uint8_t *)realloc(sieve, (size_t)(limit + 1));
    if (!new_sieve)
        exit(1);
    sieve = new_sieve;

    /* Mark 0 and 1 as composite */
    if (limit >= 0) sieve[0] = 1;
    if (limit >= 1) sieve[1] = 1;

    /* Initialize new entries to 0 (prime) */
    for (uint32_t i = max_sieved + 1; i <= limit; ++i)
        sieve[i] = 0;

    /* Sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Rebuild prime list */
    free(primes);
    primes = NULL;
    prime_count = 0;
    prime_cap = limit + 1;   /* upper bound on number of primes up to limit */
    primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!primes)
        exit(1);

    for (uint32_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            primes[prime_count++] = i;

    max_sieved = limit;
}

/* Test primality of a 32‑bit number */
static bool isPrime_uint32(uint32_t x)
{
    if (x <= max_sieved)
        return !sieve[x];

    uint32_t r = (uint32_t)sqrt((double)x);
    ensure_sieve_upto(r);
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if (p > r)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Count how many primes are strictly less than n (n >= 2) */
static uint32_t primeCountBelow(uint32_t n)
{
    ensure_sieve_upto(n - 1);
    size_t left = 0, right = prime_count;
    while (left < right) {
        size_t mid = (left + right) / 2;
        if (primes[mid] < n)
            left = mid + 1;
        else
            right = mid;
    }
    return (uint32_t)left;
}

/* Main function requested by the specification */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 as per offset 2 */

    uint32_t n32 = (uint32_t)n;
    uint32_t required = primeCountBelow(n32);

    /* If there are no primes below n, the answer is simply n + 1 */
    if (required == 0)
        return (seq_t)(n32 + 1);

    uint32_t x = n32 + 1;
    uint32_t composites = 0;

    while (composites < required) {
        bool compositeFlag;
        if (x <= max_sieved)
            compositeFlag = (sieve[x] != 0);
        else
            compositeFlag = !isPrime_uint32(x);

        if (compositeFlag) {
            ++composites;
            if (composites == required)
                break;
        }
        ++x;
    }

    return (seq_t)(x + 1);
}
