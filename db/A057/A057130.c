/*
 * A057130  Product of first n primes of form 6k-1.
 * https://oeis.org/A057130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of first n primes that are ≡5 (mod 6) * a(1) = 5,
 *            a(n) = a(n-1) * p_n where p_n is the nth such prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache, is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t n); */
#include "../utils/seq_mul.h"       /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/ensure_cache.h"/* void ensure_cache(idx_t n, seq_t **cache, size_t *cap); */

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) /* safety for non‑offset calls */
        return 0;

    /* Cache for previously computed values – 1‑based indices. */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;
    static idx_t  cache_max    = 0;   /* highest n that has been computed */

    /* Last prime of the form 6k‑1 that was used. */
    static uint64_t last_prime = 0;   /* 0 indicates none yet */

    /* Current product of primes found so far. */
    static seq_t product = 1;         /* product of primes[1..cnt] */

    /* Make sure cache can hold up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already available. */
    if (n <= cache_max)
        return cache[n];

    /* Initialise cache[0] to zero for safety */
    if (cache[0] == 0)
        cache[0] = 0;

    /* Extend sequence up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Find next prime ≡5 (mod 6) */
        uint64_t candidate;
        if (last_prime == 0)
            candidate = 5;
        else
            candidate = last_prime + 6;

        while (!is_prime(candidate))
            candidate += 6;

        last_prime = candidate;

        /* Update running product */
        product = seq_mul(product, (seq_t)candidate);

        /* Store result in cache */
        cache[i] = product;
    }

    cache_max = n;
    return cache[n];
}
