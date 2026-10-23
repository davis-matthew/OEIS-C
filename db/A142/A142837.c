/*
 * A142837  Primes congruent to 39 mod 61.
 * https://oeis.org/A142837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 79, u32 n <= 80, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142837
   Formula: a(n) = the n-th prime p such that p ≡ 39 (mod 61)   (n ≥ 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for found primes; thread‑unsafe but acceptable for
       the protocol. */
    static seq_t *cache      = NULL;   /* array of primes               */
    static size_t cache_size = 0;      /* number cached so far          */
    static size_t cache_cap  = 0;      /* allocated capacity            */

    /* next candidate number ≡ 39 (mod 61) */
    static uint64_t candidate = 39;      /* first value of the arithmetic progression */

    /* OEIS is 1‑based; a(0) is unused, but return 0 for safety. */
    if (n == 0) return 0ull;

    /* grow cache until it contains at least n primes */
    while (cache_size < (size_t)n)
    {
        /* test the current candidate */
        if (is_prime_big((seq_t)candidate))
        {
            /* allocate more space if needed */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        /* advance to next number ≡ 39 (mod 61) */
        candidate += 61;
    }

    return cache[n - 1];
}
