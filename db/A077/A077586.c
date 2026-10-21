/*
 * A077586  a(n) = 2^(2^prime(n) - 1) - 1.
 * https://oeis.org/A077586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^prime(n) - 1) - 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

static idx_t *prime_cache = NULL;   /* cache of primes */
static size_t prime_count = 0;      /* number of primes cached */

/* Ensure we have at least n primes stored in prime_cache */
static void ensure_primes(idx_t n) {
    if (prime_count >= (size_t)n) return;

    /* Rough upper bound for the nth prime: n * (log n + log log n) */
    /* For simplicity, use n * 20 which is safe for n <= 10000 */
    idx_t limit = n < 100 ? 100 : n * 20;
    idx_t *new_cache = NULL;
    size_t new_count;

    /* Allocate and fill the new prime cache */
    build_primes(limit, &new_cache, &new_count);

    /* If we didn't get enough primes (rare for very large n), increase limit */
    while (new_count < (size_t)n) {
        limit *= 2;
        free(new_cache);
        build_primes(limit, &new_cache, &new_count);
    }

    /* Replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_count = new_count;
}

/* Public entry point requested by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range, return 0 */

    ensure_primes(n);

    /* Fetch the nth prime (prime_cache is zero‑indexed) */
    idx_t p = prime_cache[n - 1];

    /* Compute 2^p */
    seq_t pow_p = big_pow((seq_t)2, (seq_t)p);

    /* exponent = 2^p - 1 */
    seq_t exponent = pow_p - 1;

    /* Compute 2^(2^p - 1) */
    seq_t result = big_pow((seq_t)2, exponent) - 1;

    return result;
}
