/*
 * A232558  a(1)=5, q=a(n) is the smallest prime > a(n-1) such that q-2*n = p
 *          prime.
 * https://oeis.org/A232558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1367, u32 n <= 411, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5; for n>1, a(n) = the smallest prime q > a(n‑1)
   such that q-2*n is prime.  The sequence is indexed from 1. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Return the n‑th term of sequence A232558.  idx_t and seq_t are
   supplied by the build system. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms, 1‑based.  cache[0] is unused. */
    static seq_t *cache   = NULL;
    static size_t  alloc  = 0;   /* number of elements allocated */
    static idx_t   max_n  = 0;   /* largest index already computed */

    if (n == 0)          /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Ensure we have space for indices 1 .. n.  Use seq_alloc_new_array
       because seq_t may be an arbitrary‑precision class. */
    if (alloc < (size_t)(n + 1)) {
        seq_t *new_cache = seq_alloc_new_array(n + 1);
        if (new_cache == NULL) {
            /* allocation failed – give up */
            return (seq_t)0;
        }
        /* copy existing contents */
        if (cache != NULL)
            memcpy(new_cache, cache, alloc * sizeof(seq_t));
        /* free old array */
        if (cache != NULL)
            seq_free_array(cache);
        cache = new_cache;
        alloc = n + 1;
    }

    /* Initialise the first term if this is the first call. */
    if (max_n == 0) {
        cache[0] = (seq_t)0;   /* unused, but keeps indices – 1 based */
        cache[1] = (seq_t)5;   /* a(1)=5 */
        max_n   = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* previous term */
        seq_t prev = cache[k - 1];

        /* candidate prime > a(k-1) */
        uint64_t q = next_prime((uint64_t)prev);

        /* find the first q such that q-2*k is prime */
        for (;;) {
            seq_t p = (seq_t)q - (seq_t)(2 * k);   /* p = q - 2*k */
            if (is_prime_big(p)) {
                cache[k] = (seq_t)q;               /* store a(k) */
                break;
            }
            q = next_prime(q);                    /* try next prime */
        }
    }

    max_n = n;
    return cache[n];
}
