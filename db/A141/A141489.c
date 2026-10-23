/*
 * A141489  Numbers k such that k^2 + k + 257 is prime.
 * https://oeis.org/A141489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that k^2 + k + 257 is prime
   (OEIS A141489).  The function searches increasing k values
   and stores the found ones in a static cache so that any order
   of calls returns the correct term. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* unused; offset starts at 1 */

    /* static cache of the k values that satisfy the condition */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of elements currently stored   */
    static size_t cap   = 0;   /* current allocated capacity in elements*/
    static idx_t  next_k = 0;  /* next integer to test                */

    /* grow cache until it contains at least n elements */
    while (len < n) {
        seq_t k = (seq_t)next_k;               /* candidate integer       */
        seq_t val = k * k + k + (seq_t)257;    /* k^2 + k + 257           */

        if (isPrimeGeneric(val)) {             /* test primality          */
            if (len == cap) {                 /* enlarge storage as needed */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = k;                  /* store found k           */
        }
        ++next_k;                              /* try next integer        */
    }

    return cache[n - 1];                       /* return the nth term     */
}
