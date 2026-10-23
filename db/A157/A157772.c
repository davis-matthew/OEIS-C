/*
 * A157772  Numbers n such that 100n + 13 is prime.
 * https://oeis.org/A157772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A157772: numbers n such that 100*n+13 is prime. */
/* Algorithm: brute‑force search with memoised cache. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*  static cache of found terms:
            cache[i] == a(i+1)  (offset 1)          */
    static seq_t *cache  = NULL;
    static idx_t  len    = 0;     /* number of terms stored   */
    static idx_t  cap    = 0;     /* current capacity of cache*/
    static idx_t  cand   = 0;     /* next m to test            */

    if (n == 0) return 0; /* undefined for n<1, harmless    */

    /* Grow cache until it contains at least n terms */
    while (len < n) {
        idx_t p = cand * 100ULL + 13ULL;      /* number to test for primality */
        if (is_prime(p)) {                    /* if 100*cand+13 is prime   */
            if (len >= cap) {                /* enlarge the cache           */
                idx_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* assume allocation succeeds            */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)cand;    /* store the term                 */
        }
        cand++;                           /* next candidate                 */
    }

    return cache[n - 1];
}
