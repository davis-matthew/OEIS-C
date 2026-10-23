/*
 * A133630  Nonprime numbers k such that binomial(k+p,k) mod k = 1, where
 *          p=10.
 * https://oeis.org/A133630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 9719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom10_mod_k, is_prime, ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the nth non‑prime integer k such that
   binomial(k+10,k) ≡ 1 (mod k).  The function
   searches candidates, tests the condition, and
   caches found terms to answer any n efficiently.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/binom10_mod_k.h"
#include "../utils/ugcd_uint64.h"

/* Euclidean gcd for 64‑bit unsigned integers. */

seq_t a(idx_t n)
{
    /* Cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  found  = 0;
    static idx_t  next_k = 4;   /* first candidate */

    while (found < n) {
        if (found == cap) {                 /* grow cache storage */
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap   = new_cap;
        }
        /* Find the next admissible non‑prime k. */
        for (;;) {
            if (!is_prime(next_k) &&
                binom10_mod_k(next_k) == (seq_t)1) {
                break;
            }
            ++next_k;
        }
        cache[found++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
