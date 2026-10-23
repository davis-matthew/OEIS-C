/*
 * A122220  a(n) = (prime(n)^6 - prime(n)^2)/20.
 * https://oeis.org/A122220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^6 - prime(n)^2)/20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

/* Cache for the first n primes */
static uint32_t *prime_cache = NULL;
static size_t prime_count = 0;

/* Ensure that the cache contains at least the first n primes */
static void ensure_primes(idx_t n)
{
    if (prime_cache && prime_count >= (size_t)n)
        return;                     /* already have enough */

    /* Release old cache if it exists */
    if (prime_cache) {
        free(prime_cache);
        prime_cache = NULL;
        prime_count = 0;
    }

    /* Build the first n primes */
    sieve_first_n_primes(n, &prime_cache, &prime_count);
}

/* Compute the n-th term of sequence A122220 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset 1 – guard against 0 */

    ensure_primes(n);                /* make sure we have prime(n) */

    uint32_t p = prime_cache[(size_t)(n - 1)];    /* 0‑based indexing */
    seq_t p_seq = (seq_t)p;

    seq_t p2 = p_seq * p_seq;        /* p^2 */
    seq_t p4 = p2 * p2;              /* p^4 */
    seq_t p6 = p4 * p2;              /* p^6 */

    return (p6 - p2) / 20;           /* (p^6 - p^2)/20 */
}
