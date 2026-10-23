/*
 * A045433  Primes congruent to {1, 3, 6} mod 7.
 * https://oeis.org/A045433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {1, 3, 6} mod 7.  a(1) = 3.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p with p ≡ 1, 3 or 6 (mod 7).  Indexing starts at 1.  */
seq_t a(idx_t n)
{
    /* static cache for found primes */
    static seq_t *cache = NULL; /* pointer to array of seq_t */
    static size_t cap   = 0;
    static size_t sz    = 0;   /* number of primes stored */
    static uint64_t cand = 3;  /* next odd candidate to test */

    /* initialise cache if not done yet */
    if (!cache) {
        cap   = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* generate primes until we have at least n of them */
    while (sz < n) {
        if (is_prime(cand)) {
            uint64_t r = cand % 7;
            if (r == 1 || r == 3 || r == 6) {
                if (sz == cap) {            /* grow cache if needed */
                    cap *= 2;
                    cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                }
                cache[sz++] = (seq_t)cand;
            }
        }
        cand += 2;                         /* skip even numbers */
    }

    return cache[n - 1];
}
