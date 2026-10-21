/*
 * A061265  Number of squares between n-th prime and (n+1)st prime.
 * https://oeis.org/A061265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(sqrt(prime(n+1))) - floor(sqrt(prime(n)))
 * Range:     u8 n <= 53, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   int_sqrt_uint64, sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes primes on first use by a simple sieve and
   uses integer square roots implemented in this file. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt_uint64.h"
#include "../utils/sqrt_seq.h"

/* How many primes are required for n up to 10000 (n+1th prime). */
#define REQ_PRIMES 12000
/* Upper bound for the sieve – sufficient for 12000 primes. */
#define SIEVE_LIMIT 200000

/* Static arrays to hold the prime list. */
static uint32_t primes[REQ_PRIMES];
static size_t primes_cnt = 0;

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes: generates the first REQ_PRIMES primes.     */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes_cnt > 0)        /* already initialised */
        return;

    /* Allocate a boolean sieve array. */
    bool *sieve = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve) abort();       /* cannot continue */

    /* Mark all numbers ≥2 as prime initially. */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        sieve[i] = true;

    /* Eratosthenes: cross out multiples. */
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Store primes until we have the required count. */
    for (uint32_t i = 2; i <= SIEVE_LIMIT && primes_cnt < REQ_PRIMES; ++i) {
        if (sieve[i])
            primes[primes_cnt++] = i;
    }

    free(sieve);

    if (primes_cnt < REQ_PRIMES) abort();  /* should never happen */
}

/* ------------------------------------------------------------------ */
/* Integer square root for a 64‑bit unsigned integer.                 */

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based and n ≥ 1.  Initialise primes on first call. */
    init_primes();

    /* Access primes[n‑1] and primes[n] safely. */
    uint32_t p_n    = primes[n - 1];
    uint32_t p_next = primes[n];

    /* Compute integer square roots and return their difference. */
    seq_t s_n    = sqrt_seq((seq_t)p_n);
    seq_t s_next = sqrt_seq((seq_t)p_next);
    return s_next - s_n;
}
