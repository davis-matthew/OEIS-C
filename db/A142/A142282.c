/*
 * A142282  Primes congruent to 33 mod 43.
 * https://oeis.org/A142282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 33 mod 43 (OEIS A142282)
 * Range:     u8 none, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* sequence undefined for n==0, return 0 */
        return 0;

    static seq_t *cache = NULL;   /* array of found primes   */
    static size_t len   = 0;      /* number of stored primes */
    static size_t cap   = 0;      /* capacity of cache       */
    static uint64_t next_candidate = 33; /* first number of form 43*k+33 */

    /* Grow cache until it contains at least n primes. */
    while (len < n) {
        uint64_t cand = next_candidate;

        /* Search for the next prime of the form 43*k+33. */
        while (1) {
            if (is_prime((idx_t)cand)) {  /* prime found */
                if (len == cap) {         /* enlarge cache if needed */
                    size_t new_cap = cap ? cap * 2 : 32;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {           /* allocation failed: clean up */
                        free(cache);
                        cache = NULL;
                        len = cap = 0;
                        return 0;
                    }
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = (seq_t)cand;
                next_candidate = cand + 43;   /* next candidate one cycle later */
                break;
            }
            cand += 43;   /* try next number of the form 43*k+33 */
        }
    }

    return cache[n - 1];
}
