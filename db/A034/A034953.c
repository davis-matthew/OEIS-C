/*
 * A034953  Triangular numbers (A000217) with prime indices.
 * https://oeis.org/A034953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) * (p(n) + 1) / 2, where p(n) is the n‑th prime
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 6602, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* Compute the n-th term of OEIS A034953 (triangular numbers with prime indices). */
seq_t a(idx_t n)
{
    /* Guard against the zero index; the sequence is defined for n >= 1. */
    if (n == 0) return 0;

    /* Static cache of the first N primes generated on demand. */
    static uint32_t *prime_cache = NULL;
    static idx_t cache_size = 0;      /* number of primes currently cached */

    /* Extend the cache if we do not yet have enough primes. */
    if (n > cache_size) {
        uint32_t *new_cache = NULL;
        size_t new_cnt = 0;
        sieve_first_n_primes(n, &new_cache, &new_cnt);
        /* Replace the old cache (freeing it first). */
        free(prime_cache);
        prime_cache = new_cache;
        cache_size = (idx_t)new_cnt;
    }

    /* Retrieve the (n‑1)-st element because the cache is zero‑based. */
    seq_t p = (seq_t)prime_cache[n - 1];
    return p * (p + 1) / 2;
}
