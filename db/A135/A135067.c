/*
 * A135067  Palindromic cubes p^3, where p is a prime.
 * https://oeis.org/A135067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base
 * Formula:   a(n) = p(n)^3 where p(n) is the nth prime whose cube is
 *            palindromic
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   append_candidate, big_pow, is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates primes, tests for palindromic cubes,
   and stores results in a dynamic cache to support arbitrary call ordering. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime.h"
#include "../utils/is_palindrome.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Handle offset: valid indices start at 1. */
    if (n < 1) return (seq_t)0;

    /* Static cache for the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    /* Prime candidate generator state. */
    static uint64_t next_prime = 2;
    static bool initialized = false;

    /* Initialise cache on first call. */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        initialized = true;
    }

    /* Extend cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)next_prime)) {
            seq_t p = (seq_t)next_prime;
            seq_t cube = big_pow(p, (seq_t)3);   /* p^3 */

            if (is_palindrome(cube, (idx_t)10))
                append_candidate(&cache, &cache_len, &cache_cap, cube);
        }
        next_prime++;
    }

    return cache[n - 1];
}
