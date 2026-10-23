/*
 * A156549  Race between primes having an odd/even number of zeros in their
 *          binary representation.
 * https://oeis.org/A156549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3991, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve primes up to prime(n) and count parity of zeros in
   binary representation.  a(n) = (# primes ≤ prime(n) having an odd
   number of zeros) – (# primes ≤ prime(n) having an even number of
   zeros).  The sieve is rebuilt only when a larger n is requested. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache of primes for the largest n that has been required so far. */
static uint64_t *cached_primes = NULL;  /* array of primes */
static size_t   cached_count = 0;       /* number of primes stored */

/* Helper to generate the first n primes by a simple sieve. */
static void generate_first_n_primes(idx_t n)
{
    /* Upper bound for the nth prime – safe for n up to at least 10000. */
    idx_t limit = n * 20 + 10;          /* e.g. 10000 → 200010 > 104729 */

    /* Allocate a bitmap for the sieve. */
    size_t sz = (size_t)limit + 1;
    uint8_t *is_prime = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!is_prime)
        return;                        /* allocation failure – give up */

    /* Sieve of Eratosthenes. */
    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = 1;
    for (size_t i = 2; (uint64_t)i * i < limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < sz; j += i)
                is_prime[j] = 0;
        }
    }

    /* Store primes in a new array (resize as needed). */
    size_t capacity = n + 100;         /* small safety margin */
    uint64_t *primes = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return;                        /* allocation failure – give up */
    }

    size_t idx = 0;
    for (size_t i = 2; i < sz && idx < (size_t)n; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
        }
    }

    /* Free old cache and install new one. */
    free(cached_primes);
    cached_primes = primes;
    cached_count  = idx;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n = 0, but harmless */

    /* Ensure we have at least n primes in the cache. */
    if (cached_count < (size_t)n)
        generate_first_n_primes(n);

    /* Count primes with odd vs. even number of zeros in binary. */
    seq_t odd  = 0;
    seq_t even = 0;

    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t p = cached_primes[i];

        /* Number of bits in p (most significant bit position + 1). */
        unsigned int bits = 64 - __builtin_clzll(p);
        unsigned int ones = __builtin_popcountll(p);
        unsigned int zeros = bits - ones;

        if (zeros & 1)
            odd++;
        else
            even++;
    }

    return odd - even;
}
