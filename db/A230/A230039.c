/*
 * A230039  Primes p such that 2*p+1 is not prime and 2*p+3 is prime.
 * https://oeis.org/A230039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 852, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 2*p+1 is composite and 2*p+3 is prime. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Cached sequence data. */
static seq_t *cache      = NULL;   /* array of the sequence terms   */
static size_t cache_len  = 0;      /* number of terms stored       */
static size_t cache_cap  = 0;      /* current capacity of cache    */
static idx_t  next_cand  = 2;      /* next integer to test for primality */

/* Ensure the cache holds at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate */
        for (;;) {
            seq_t cand = (seq_t)next_cand;
            if (!is_prime_small(cand)) {                /* not prime */
                ++next_cand;
                continue;
            }

            /* 2*p+1 must be composite, 2*p+3 must be prime */
            seq_t two_p_plus1 = ((seq_t)next_cand << 1) + 1;
            seq_t two_p_plus3 = ((seq_t)next_cand << 1) + 3;
            if (!is_prime_small(two_p_plus1) &&
                is_prime_small(two_p_plus3)) {
                /* Store the prime */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = cand;
                ++next_cand;   /* advance for future searches */
                break;         /* move to next candidate for future calls */
            }
            ++next_cand;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1: a(1) is the first term */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
