/*
 * A045381  Primes congruent to {0, 1, 2} mod 7.
 * https://oeis.org/A045381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: return the n‑th prime that is ≡0,1 or 2 (mod 7).
   Matches OEIS sequence A045381. */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t next_candidate = 3; /* start looking at odd numbers >2 */

    if (n == 0) return (seq_t)0; /* undefined below offset, but return 0 */

    /* Ensure cache has enough capacity for new entries */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* cannot proceed */
    }

    /* Extend cache until it contains at least n elements */
    while (cache_size < n) {
        /* First element is always the prime 2 */
        if (cache_size == 0) {
            cache[0] = (seq_t)2;
            cache_size = 1;
            continue;
        }

        /* Grow cache if full */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next prime ≡0,1,2 (mod 7) */
        for (;;) {
            if ((next_candidate % 7) <= 2 && is_prime(next_candidate)) {
                cache[cache_size++] = (seq_t)next_candidate;
                next_candidate += 2;      /* move to next odd candidate */
                break;
            }
            next_candidate += 2;          /* skip even numbers */
        }
    }

    return cache[n - 1];
}
