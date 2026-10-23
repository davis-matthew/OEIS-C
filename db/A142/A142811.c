/*
 * A142811  Primes congruent to 13 mod 61.
 * https://oeis.org/A142811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   this function computes a(n) = the nth prime congruent to 13 (mod
 *            61)
 * Range:     u8 n <= 1, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It searches successive candidates of the form 61*k+13 and stores the primes
   in a static cache so that repeated calls do not redo work.  The implementation
   uses the provided is_prime(idx_t) routine. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;    /* number of primes stored */
    static uint64_t last_k = 0; /* k of the last examined candidate */

    if (cache == NULL) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)13;         /* first prime of form 61*k+13 is 13 */
        count = 1;
        last_k = 0;
    }

    /* generate primes until we have at least n of them */
    while (count < n) {
        uint64_t k   = last_k + 1;
        uint64_t cand = 61 * k + 13;           /* candidate prime */
        if (is_prime((idx_t)cand)) {
            /* grow cache if necessary */
            if (count >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count] = (seq_t)cand;
            count++;
        }
        last_k = k;
    }

    return cache[n - 1];
}
