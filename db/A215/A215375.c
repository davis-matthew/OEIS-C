/*
 * A215375  Primes congruent to {0, 2, 3} mod 13.
 * https://oeis.org/A215375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to {0, 2, 3} mod 13
 * Range:     u8 n <= 10, u16 n <= 837, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache and a simple primality test */
/* (is_prime_big) to generate the sequence on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached prime values       */
static size_t cache_cap = 0;         /* allocated capacity        */
static idx_t  cache_len = 0;         /* number of cached values   */
static seq_t  next_candidate = 2;    /* next integer to test      */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* OEIS offset is 1 */

    /* extend cache until it holds at least n entries */
    while (cache_len < n) {
        seq_t cand = next_candidate;

        if (is_prime_big(cand)) {
            seq_t r = cand % 13;
            if (r == 0 || r == 2 || r == 3) {
                /* grow cache if necessary */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {        /* allocation failure */
                        cache = NULL;
                        cache_cap = 0;
                        cache_len = 0;
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
            }
        }

        /* advance to next candidate: 2 → 3, afterwards only odds */
        if (cand == 2)
            next_candidate = (seq_t)3;
        else
            next_candidate = cand + (seq_t)2;
    }

    return cache[n - 1];
}
