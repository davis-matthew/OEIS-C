/*
 * A045427  Primes congruent to {3, 4, 6} mod 7.
 * https://oeis.org/A045427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p mod 7 ∈ {3,4,6}
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t next_candidate = 3;   /* first integer to examine */

/* Grow the cache array when its capacity is reached. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Return the n-th prime that is congruent to 3, 4 or 6 modulo 7. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour below not used */

    /* Extend the cache until it contains at least n qualifying primes. */
    while (cache_len < n) {
        for ( ; ; ++next_candidate) {
            if (!is_prime(next_candidate)) continue;
            idx_t r = next_candidate % 7;
            if (r == 3 || r == 4 || r == 6) {
                if (cache_len == cache_cap) grow_cache();
                prime_cache[cache_len++] = (seq_t)next_candidate;
                /* Advance to the next number for future searches. */
                ++next_candidate;
                break;           /* found the next required prime */
            }
        }
    }
    return prime_cache[n - 1];
}
