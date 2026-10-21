/*
 * A215320  Primes congruent to {1, 2, 3, 5, 6} mod 17.
 * https://oeis.org/A215320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes congruent to {1,2,3,5,6} modulo 17.
   The sequence is defined by a(1)=2, a(2)=3, a(3)=5, …   */

/* We search by testing successive integers, keeping a
   cache of all qualifying primes found so far.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for terms that have already been found */
    static seq_t *cache  = NULL; /* array of found primes          */
    static size_t  cap    = 0;   /* allocated capacity              */
    static size_t  size   = 0;   /* number of terms currently in cache */
    static idx_t   last   = 1;   /* last integer tested (will be incremented) */

    /* a(0) is undefined, but we return 0 safely */
    if (n == 0) return (seq_t)0;

    /* Keep generating terms until we have at least n of them */
    while (size < n) {
        /* Determine next candidate integer */
        for (;;) {
            ++last;                       /* test next integer */
            if (!is_prime(last))          /* skip non‑prime */
                continue;
            idx_t r = last % 17;
            if (r == 1 || r == 2 || r == 3 || r == 5 || r == 6) {
                /* Ensure we have enough space in the cache */
                if (size == cap) {
                    size_t newcap = (cap == 0) ? 16 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  newcap * sizeof(seq_t));
                    if (!tmp) {            /* allocation failed */
                        /* In the unlikely event of OOM, return 0 */
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cap   = newcap;
                }
                cache[size++] = (seq_t)last; /* store the prime */
                break;                     /* found next term */
            }
        }
    }

    /* The nth term is now in the cache */
    return cache[n - 1];
}
