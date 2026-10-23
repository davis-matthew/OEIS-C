/*
 * A180939  Numbers n such that n^2 - 2999n + 2248541 is prime.
 * https://oeis.org/A180939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer n such that n^2 - 2999*n + 2248541 is
 *            prime
 * Range:     u8 n <= 2, u16 none, u32 none, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the nth term of A180939.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t sz   = 0;       /* number of cached terms */
    static size_t cap  = 0;       /* allocated capacity of cache */
    static idx_t  next_candidate = 1;   /* next integer to test */

    /* Ensure the cache contains at least n terms. */
    while (sz < n) {
        idx_t cand = next_candidate++;   /* candidate integer */

        /* Evaluate the quadratic polynomial in seq_t arithmetic. */
        seq_t val = (seq_t)cand * (seq_t)cand
                  - ((seq_t)2999 * (seq_t)cand)
                  + (seq_t)2248541;

        /* Test for primality. */
        if (is_prime_big(val)) {
            /* Grow the cache if full. */
            if (sz == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = (seq_t)cand;   /* store the integer itself */
        }
    }

    return cache[(size_t)(n - 1)];
}
