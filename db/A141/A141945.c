/*
 * A141945  Primes congruent to 23 mod 25.
 * https://oeis.org/A141945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 328, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of prime numbers whose last two decimal digits
   are 23 or 73.  We generate primes of the form p = 25·k + 23
   (congruence 23 (mod 25)) and return the n‑th such prime. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility for primality testing of arbitrary‑size integers */
#include "../utils/is_prime_big.h"

/* Return the n‑th prime p such that p % 25 == 23 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of previously found primes */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;
    static idx_t  last_k    = 0;     /* k value of the last tested candidate */
    
    /* Allocate initial cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz = 0;
        last_k = 0;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Generate further primes until we reach the desired index */
    while (cache_sz < n) {
        seq_t cand = (seq_t)last_k * 25 + 23;
        if (is_prime_big(cand)) {
            /* Grow cache if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (new_cache == NULL) /* allocation failure; abort */
                    return (seq_t)0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = cand;
        }
        last_k++;
    }
    return cache[n - 1];
}
