/*
 * A132234  Primes congruent to 19 (mod 30).
 * https://oeis.org/A132234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 802, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 19 (mod 30) by incremental trial
 * testing of numbers of the form 30k + 19.  The function caches
 * found terms so that calls can be made in any order. */
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* next candidate of the form 30k+19 to test */
    static seq_t next_candidate = 19;

    /* extend cache until we have at least n terms */
    while (len < n) {
        /* test whether current candidate is prime */
        if (is_prime_small(next_candidate)) {
            /* grow the dynamic array if necessary */
            if (len == cap) {
                size_t new_cap   = (cap == 0) ? 8 : cap << 1;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* allocation failure – terminate */
                    exit(EXIT_FAILURE);
                }
                cache = new_cache;
                cap   = new_cap;
            }
            cache[len++] = next_candidate;
        }
        /* advance to the next number of the form 30k+19 */
        next_candidate += 30;
    }
    return cache[n - 1];
}
