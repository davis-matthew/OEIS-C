/*
 * A179545  The sum of the elements within a jump in a Sieve of Eratosthenes
 *          table.
 * https://oeis.org/A179545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * p(n) * (p(n)-1) / 2 where p(n) is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 5454, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* With zero-indexing in the array, a(1) corresponds to the first prime 2 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes; initially empty */
static uint64_t *prime_array = NULL;
static size_t prime_count  = 0;      /* number of primes currently stored */

/* Rough upper bound for the n-th prime (sufficient for n ≤ 10000) */
static inline idx_t prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;       /* 6th prime is 13 */
    return 20 * n + 100;        /* ample room for all required n */
}

/* Ensure that prime_array contains at least n primes */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_count) return; /* already have enough */

    idx_t upper = prime_upper_bound(n);
    bool *sieve = (bool *)calloc((size_t)(upper + 1), sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    sieve[0] = sieve[1] = true;  /* 0 and 1 are not prime */
    for (idx_t i = 2; i * i <= upper; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= upper; j += i)
                sieve[j] = true;
        }
    }

    /* Count how many primes we found */
    size_t cnt = 0;
    for (idx_t i = 2; i <= upper; ++i)
        if (!sieve[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(sieve);
        exit(EXIT_FAILURE);
    }

    /* Fill the new prime array */
    size_t idx = 0;
    for (idx_t i = 2; i <= upper; ++i)
        if (!sieve[i]) new_primes[idx++] = (uint64_t)i;

    /* Replace the old array */
    free(prime_array);
    free(sieve);
    prime_array = new_primes;
    prime_count = cnt;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* Undefined for n = 0, but harmless */

    ensure_primes(n);                     /* make sure nth prime is present */
    seq_t p = (seq_t)prime_array[n - 1];   /* nth prime */

    seq_t result = p * (p - 1);
    result = result / 2;
    result = result * 3;

    return result;
}
