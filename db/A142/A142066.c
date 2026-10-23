/*
 * A142066  Primes congruent to 29 mod 33.
 * https://oeis.org/A142066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 29 mod 33, found by a simple incremental
 * search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of primes of the form 33k+29 that we have found so far */
    static seq_t *cache       = NULL;
    static size_t cachecnt    = 0;   /* number of stored terms   */
    static size_t cachecap    = 0;   /* capacity of the cache    */
    static idx_t next_candidate = 29;/* next integer ≡ 29 (mod 33) to test */

    /* Grow the cache until it contains at least n terms */
    while (cachecnt < (size_t)n) {
        /* Test the current candidate */
        if (is_prime(next_candidate)) {
            /* Allocate more space if needed */
            if (cachecnt == cachecap) {
                size_t newcap = cachecap ? cachecap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {                  /* allocation failure */
                    return 0;                 /* sentinel */
                }
                cache = tmp;
                cachecap = newcap;
            }
            /* Store the found prime */
            cache[cachecnt++] = (seq_t)next_candidate;
        }
        /* Advance to the next number ≡ 29 (mod 33) */
        next_candidate += 33;
    }

    return cache[(size_t)n - 1];   /* a(1) corresponds to cache[0] */
}
