/*
 * A120628  Primes such that their double is 1 away from a prime number.
 * https://oeis.org/A120628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 721, u32 n <= 942, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 2p±1 is prime (OEIS A120628) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* sequence cache             */
static size_t cache_size = 0;      /* number of elements cached  */
static size_t cache_cap  = 0;      /* allocated capacity         */
static idx_t  next_candidate = 2;  /* next integer to test       */

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n elements   */
    while (cache_size < n) {
        /* Check if next_candidate is a prime                 */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t dbl = (seq_t)next_candidate * 2;
            /* 2p-1 or 2p+1 must be prime                       */
            if (is_prime_big(dbl - 1) || is_prime_big(dbl + 1)) {
                /* Ensure capacity for the new element          */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { exit(EXIT_FAILURE); }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }

        /* Advance to the next odd number (after 2)           */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
    return cache[n - 1];
}
