/*
 * A092769  Squares of A006450: a(n) = prime(prime(n))^2.
 * https://oeis.org/A092769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n))^2 where prime(k) is the k‑th prime
 *            (prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 844, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIEVE_LIMIT 2000000   /* sufficient for n up to about 10000 */
#define MAX_PRIMES  200000   /* > number of primes below SIEVE_LIMIT */

static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;
static bool primes_ready = false;

/* Build the list of primes up to SIEVE_LIMIT once. */
static void init_primes(void)
{
    if (primes_ready)
        return;

    /* using a simple Eratosthenes sieve with a bitset */
    uint8_t *is_composite = (uint8_t*)calloc(SIEVE_LIMIT + 1, 1);
    /* 0 and 1 are not prime */
    is_composite[0] = is_composite[1] = 1;

    for (uint32_t i = 2; ((uint64_t)i * i) <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count < MAX_PRIMES)
                primes[prime_count++] = i;
        }
    }
    free(is_composite);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for n==0, return 0 */
        return (seq_t)0;

    init_primes();

    /* the n-th prime (1‑based index) */
    uint32_t p_n;
    if (n-1 < prime_count)
        p_n = primes[n-1];
    else
        return (seq_t)0; /* out of range for this implementation */

    /* the prime at index p_n (again 1‑based) */
    uint32_t p_pp;
    if (p_n-1 < prime_count)
        p_pp = primes[p_n-1];
    else
        return (seq_t)0; /* out of range */

    /* square the prime.  The product fits in 64 bits for the expected range. */
    seq_t result = (seq_t)p_pp * p_pp;
    return result;
}
