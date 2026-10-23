/*
 * A142224  Primes congruent to 27 mod 41.
 * https://oeis.org/A142224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 27 mod 41.
   a(n) = the nth prime p such that p ≡ 27 (mod 41) (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* memoised cache of discovered primes */
static seq_t *cache = NULL;      /* array of primes */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t   cache_len = 0;    /* number of primes stored in cache */
static idx_t   next_candidate = 27;  /* next number to test (≡27 mod 41) */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n<1 */
    if (n <= cache_len) return cache[n - 1];

    /* grow the cache if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate further primes until we have n terms */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 41;   /* advance to the next number ≡27 mod 41 */
    }

    return cache[n - 1];
}
