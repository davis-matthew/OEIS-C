/*
 * A145489  Numbers k such that 6k + 11 is prime and 12k + 5 is also prime.
 * https://oeis.org/A145489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 3834, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145489: numbers k such that 6k + 11 and 12k + 5 are prime.
   Offset: 1  (a(1) = 0). This routine returns a(n) for any n ≥ 1,
   using lazy generation and an on‑demand cache.                                      */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For the off‑by‑one offset, a(1) is the first element. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of discovered k values. */
    static seq_t *cache   = NULL;
    static size_t  len    = 0;        /* number of values stored */
    static size_t  cap    = 0;        /* allocated capacity   */
    static idx_t   next_k = 0;        /* next k to test       */

    /* Initialise cache lazily. */
    if (len == 0) {
        cap   = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Generate k values until we have at least n elements. */
    while (len < (size_t)n) {
        idx_t k  = next_k++;              /* candidate k           */
        idx_t p1 = 6 * k + 11;             /* 6k + 11              */
        idx_t p2 = 12 * k + 5;             /* 12k + 5              */

        if (is_prime(p1) && is_prime(p2)) {
            if (len == cap) {             /* grow cache if needed */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!cache)
                    abort();
            }
            cache[len++] = (seq_t)k;      /* store found k        */
        }
    }

    /* Return the (n-1)-th element (since cache is 0‑based). */
    return cache[n - 1];
}
