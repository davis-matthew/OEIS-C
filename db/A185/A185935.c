/*
 * A185935  Lesser of two consecutive primes which both equal 2 (mod 3).
 * https://oeis.org/A185935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the lesser of consecutive primes p and q
   such that p ≡ 2 (mod 3), q ≡ 2 (mod 3). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Helper: return the next prime strictly greater than cur. */
static seq_t next_prime_generic(seq_t cur)
{
    seq_t nxt = cur + 1;
    while (!is_prime_big(nxt))
        nxt += 1;
    return nxt;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Static state of the prime generator */
    static seq_t prev_prime = 0, curr_prime = 0;
    static bool initialized = false;

    /* Initialisation */
    if (!initialized) {
        prev_prime = 2;
        curr_prime = 3;          /* first prime after 2   */
        initialized = true;

        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Extend cache until we have n terms */
    while (cache_len < n) {
        /* If the consecutive pair satisfies the condition,
           store the lesser prime. */
        if ((prev_prime % 3U == 2U) && (curr_prime % 3U == 2U)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = prev_prime;
        }

        /* Advance to the next pair of consecutive primes. */
        prev_prime = curr_prime;
        curr_prime = next_prime_generic(curr_prime);
    }

    return cache[n - 1];
}
