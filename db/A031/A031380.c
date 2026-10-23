/*
 * A031380  a(n) = prime(6*n - 2).
 * https://oeis.org/A031380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(6*n - 2)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

static idx_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Ensure at least k primes are available in prime_cache. */
static void ensure_primes_up_to(idx_t k)
{
    if (prime_count >= (size_t)k)
        return;                       /* already enough */

    /* Rough upper bound for the k‑th prime: p_k < 15*k for small k. */
    idx_t limit = (idx_t)15 * k;
    if (limit < (idx_t)1000)
        limit = 1000;

    /* Increase the sieve limit until we have at least k primes. */
    while (1) {
        free(prime_cache);              /* discard old array */
        sieve_primes(limit, &prime_cache, &prime_count);
        if (prime_count >= (size_t)k)
            break;
        limit *= 2;                     /* double the bound */
    }
}

seq_t a(idx_t n)
{
    /* n ≥ 1; compute the k‑th prime with k = 6*n - 2. */
    idx_t k = (idx_t)6 * n - 2;
    ensure_primes_up_to(k);
    return (seq_t)prime_cache[k - 1];
}
