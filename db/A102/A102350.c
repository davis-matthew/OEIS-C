/*
 * A102350  Prime(144*n).
 * https://oeis.org/A102350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(144*n)
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_prime_upper_bound.h"

static uint8_t  *sieve      = NULL;   /* 0 = prime, 1 = composite */
static size_t    sieve_size = 0;
static uint32_t *primes     = NULL ;  /* list of primes found */
static size_t    primes_count   = 0;
static size_t    primes_capacity = 0;

/* Build a sieve up to an upper bound that guarantees at least
   needed_primes prime numbers.  Rebuilds the sieve from scratch.
   Assumes the estimate_prime_upper_bound() routine returns a safe bound. */
static void rebuild_sieve(idx_t needed_primes)
{
    uint64_t limit = estimate_prime_upper_bound(needed_primes);
    if (!limit || limit < 2) limit = 2;

    /* Allocate sieve array */
    if (sieve) free(sieve);
    sieve = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!sieve) { /* out‑of‑memory: abort */
        exit(1);
    }

    /* Eratosthenes sieve */
    size_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (j = i * i; j <= limit; j += i) {
                sieve[j] = 1;
            }
        }
    }

    /* Count primes and store them */
    /* About limit / log(limit) primes; add a small safety margin. */
    primes_capacity = (size_t)(limit / (log((double)limit) + 1)) + 10;
    if (primes) free(primes);
    primes = (uint32_t *)malloc(primes_capacity * sizeof(uint32_t));
    if (!primes) exit(1);

    primes_count = 0;
    for (i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (primes_count == primes_capacity) {
                primes_capacity *= 2;
                primes = (uint32_t *)realloc(primes,
                        primes_capacity * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_count++] = (uint32_t)i;
        }
    }

    sieve_size = limit + 1;
}

/* Ensure that we have at least needed_primes prime numbers in the array.
   Builds or extends the sieve if necessary. */
static void ensure_primes_at_least(idx_t needed_primes)
{
    if (needed_primes <= primes_count) return;
    rebuild_sieve(needed_primes);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of range */
    idx_t target_index = (idx_t)144 * n;
    ensure_primes_at_least(target_index);
    return (seq_t)primes[target_index - 1];
}
