/*
 * A090258  Last term of prime quadruples.
 * https://oeis.org/A090258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the last element of the nth prime quadruple (p, p+2, p+6, p+8). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n == 0 is outside the sequence; return 0 as a safety value */
    if (n == 0) return 0;

    /* static cache for previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* the next admissible start of a quadruple; starts are ≡ 5 (mod 6) */
    static idx_t next_start = 5;   /* first possible p */

    /* if we already have the result, return it */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* grow the cache if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();    /* unrecoverable memory error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* search for further quadruples until we obtain the nth term */
    while (cache_len < (size_t)n) {
        if (is_prime(next_start) &&
            is_prime(next_start + 2) &&
            is_prime(next_start + 6) &&
            is_prime(next_start + 8)) {
            cache[cache_len++] = next_start + 8;
        }
        /* move to the next admissible start (all start primes ≡ 5 mod 6) */
        next_start += 6;
    }

    return cache[n - 1];
}
