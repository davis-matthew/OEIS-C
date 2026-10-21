/*
 * A087696  Numbers n such that n + 5 and n - 5 are both prime.
 * https://oeis.org/A087696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th integer n such that n-5 and n+5 are both prime.  */
/* The sequence is defined by the property, not by a closed formula. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;   /* array of values */
    static size_t cache_len  = 0;      /* number of cached terms   */
    static size_t cache_cap  = 0;      /* allocated capacity      */

    /* the next candidate to test */
    static idx_t next_candidate = 8;    /* first integer to consider */

    /* expand cache array if needed */
    if (cache_cap == 0 && cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n */
    while (cache_len < n) {
        /* ensure current candidate is large enough that (candidate-5) >= 2 */
        if (next_candidate < 8) next_candidate = 8;

        /* test the prime condition */
        if (is_prime(next_candidate - 5) && is_prime(next_candidate + 5)) {
            /* add candidate to cache */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        /* move to the next integer */
        ++next_candidate;
    }

    return cache[n - 1];
}
