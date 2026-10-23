/*
 * A142546  Primes congruent to 16 mod 53.
 * https://oeis.org/A142546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 16 (mod 53)
 * Range:     u8 none, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(idx_t) */

static idx_t *prime_cache = NULL;   /* cache of primes ≡ 16 mod 53 */
static idx_t cached_n   = 0;        /* number of primes cached so far   */
static idx_t cache_cap  = 0;        /* current cache capacity            */
static idx_t next_candidate = 16;   /* next number to test for primality */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* offset not used, defined behaviour */

    /* Extend cache until we have at least n terms */
    while (cached_n < n) {
        if (is_prime(next_candidate)) {
            /* grow cache if needed */
            if (cached_n >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
                if (!tmp) abort(); /* allocation failed */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cached_n++] = next_candidate;
        }
        next_candidate += 53; /* next number ≡ 16 (mod 53) */
    }

    return (seq_t)prime_cache[n - 1];
}
