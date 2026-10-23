/*
 * A033215  Primes of form x^2+21*y^2.
 * https://oeis.org/A033215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 794, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes ≡ 1, 25 or 37 (mod 84), i.e. primes of the form x²+21 y².  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 5;   /* start search from the first odd number */
    static bool   initialized = false;

    if (!initialized) {
        initialized = true;
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* check congruence modulo 84 */
        idx_t r = next_candidate % 84;
        if (r == 1 || r == 25 || r == 37) {
            if (is_prime(next_candidate)) {
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }

    return cache[n - 1];
}
