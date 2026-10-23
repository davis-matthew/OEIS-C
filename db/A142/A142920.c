/*
 * A142920  Primes congruent to 55 mod 63.
 * https://oeis.org/A142920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 183, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime that is congruent to 55 modulo 63.
   It searches integers of the form 63*k + 55 and keeps only the prime ones
   in a static cache so that calls with arbitrary n (not necessarily sorted)
   are answered in O(n) time overall. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, but guard zero index */

    /* Cache for already found primes */
    static seq_t *cache = NULL;
    static idx_t len = 0;              /* number of primes cached so far */
    static idx_t cap = 0;              /* allocated cache capacity */
    static idx_t k   = 2;              /* candidate counter (63*2+55 = 181) */

    while (len < n) {
        /* Candidate number of the form 63*k + 55 */
        uint64_t cand = 63ULL * k + 55ULL;  /* 63 and 55 fit in 64 bits */
        k++;

        if (is_prime((idx_t)cand)) {
            /* Grow cache if necessary */
            if (len == cap) {
                idx_t new_cap = cap ? cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
