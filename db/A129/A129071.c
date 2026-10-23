/*
 * A129071  Numbers k such that (k-7)/6 is prime.
 * https://oeis.org/A129071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6 * prime(n) + 7, where prime(n) is the n‑th prime
 *            (1‑indexed)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/estimate_upper_bound.h"

/* Static cache for primes */
static uint64_t *primes = NULL;   /* array of primes */
static size_t prime_count = 0;    /* number of primes cached */
static idx_t sieve_limit = 0;     /* largest number sieved */

/* Build a sieve up to 'limit' and store primes in 'primes' */
static void rebuild_primes(idx_t limit) {
    /* Free previous cache */
    if (primes != NULL) {
        free(primes);
        primes = NULL;
    }

    /* Allocate sieve array */
    unsigned char *sieve = (unsigned char *)malloc((size_t)(limit + 1));
    if (sieve == NULL) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    /* Allocate and fill prime array */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (new_primes == NULL) exit(EXIT_FAILURE);
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i]) new_primes[idx++] = (uint64_t)i;
    }

    /* Replace old cache */
    primes = new_primes;
    prime_count = cnt;
    sieve_limit = limit;

    free(sieve);
}

/* Ensure that at least 'n' primes are cached */
static void ensure_primes(idx_t n) {
    if (primes != NULL && prime_count >= n) return;

    idx_t new_limit = estimate_upper_bound(n);
    /* Add margin to reduce frequent re‑sieving */
    new_limit = new_limit * 2 + 100;
    rebuild_primes(new_limit);
}

/* Public sequence function ------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n==0, but safe */
    ensure_primes(n);                   /* guarantee the n‑th prime exists */
    uint64_t p = primes[n - 1];         /* prime(n) is 1‑indexed in array */
    return (seq_t)(6 * (seq_t)p + (seq_t)7);
}
