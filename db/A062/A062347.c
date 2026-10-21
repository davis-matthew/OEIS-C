/*
 * A062347  a(n) = (product of first n primes) modulo prime(n+1).
 * https://oeis.org/A062347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of first n primes) modulo prime(n+1)
 * Range:     u8 n <= 66, u16 n <= 6628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static sieve parameters */
static bool *sieve = NULL;           /* true if composite */
static uint32_t *prime_list = NULL;  /* primes found */
static size_t prime_count = 0;
static bool primes_initialized = false;
static const size_t SIEVE_LIMIT = 200000;   /* > 10 000th prime */

/* Initialize sieve and prime list */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Allocate sieve array */
    sieve = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve)
        return;  /* allocation failure – the function will return 0 */

    /* Simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = true;

    /* Count primes up to SIEVE_LIMIT */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!sieve[i])
            ++prime_count;

    /* Allocate prime list */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) {
        free(sieve);
        sieve = NULL;
        prime_count = 0;
        return;
    }

    /* Fill prime list */
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT && idx < prime_count; ++i)
        if (!sieve[i])
            prime_list[idx++] = (uint32_t)i;

    primes_initialized = true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    init_primes();

    /* Guard against out‑of‑bounds n (should not happen with the chosen limit) */
    if ((size_t)n >= prime_count)
        return (seq_t)0;

    uint64_t mod = (uint64_t)prime_list[(size_t)n];  /* prime(n+1) */
    uint64_t prod = 1;

    for (size_t i = 0; i < (size_t)n; ++i) {
        prod = (prod * (uint64_t)prime_list[i]) % mod;
    }

    return (seq_t)prod;
}
