/*
 * A215315  Primes congruent to {2, 3, 4, 5} mod 13.
 * https://oeis.org/A215315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p with p mod 13 ∈ {2,3,4,5}. The function
 *            keeps a cache of all primes found so far and extends it as
 *            needed, so repeated calls with arbitrary n are efficient
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;     // array of found primes
static idx_t   cache_len  = 0;        // number of primes stored
static idx_t   cache_cap  = 0;        // allocated capacity
static idx_t   next_candidate = 2;    // next integer to test

static inline void grow_cache(void) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        prime_cache = new_ptr;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but harmless */

    while (cache_len < n) {
        /* test the current candidate */
        if (is_prime(next_candidate)) {
            idx_t r = next_candidate % 13;
            if (r >= 2 && r <= 5) {
                grow_cache();
                prime_cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
    return prime_cache[n - 1];
}
