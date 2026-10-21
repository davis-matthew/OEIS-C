/*
 * A092256  Nonprimes of form 6k+5.
 * https://oeis.org/A092256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 7641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************
 *  * Nonprimes of form 6k+5: a(n) is the nth composite number ≡ the form 6k+5.
 *  * The algorithm iterates over candidates 6k+5 and tests them for primality.
 *  * The function caches results so that calls are in any order.
 ********************************************/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Computes the nth term of OEIS A092256:
 * a(n) = the nth composite number congruent to 5 modulo 6. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;  /* allocated capacity */
    static size_t  cache_len = 0;  /* number of values stored */
    static seq_t next_candidate = (seq_t)5; /* next number ≡5 (mod 6) to test */

    /* extend cache until we have at least n values */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate += (seq_t)6;

        if (!isPrimeGeneric(cand)) { /* composite → add to cache */
            if (cache_len + 1 > cache_cap) {
                /* grow capacity: start at 8, then double */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* out‑of‑memory: abort (undefined behaviour in OEIS build) */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[(size_t)n - 1];
}
