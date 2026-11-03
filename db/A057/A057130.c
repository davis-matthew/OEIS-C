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
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
