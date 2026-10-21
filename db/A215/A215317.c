/*
 * A215317  Primes congruent to {2, 3, 4, 5} mod 19.
 * https://oeis.org/A215317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 2, 3, 4 or 5 modulo 19
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1; below offset is undefined */

    /* static cache of already computed terms */
    static seq_t *cache    = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    static idx_t next_candidate = 3; /* next odd number to test after 2 */
    static bool initialized = false;

    if (!initialized) {
        /* initialise cache with first term (prime 2) */
        cache_cap = 64;                               /* arbitrary small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                                 /* allocation failed */

        cache[0] = 2;                                 /* 2 ≡ 2 (mod 19) */
        cache_size = 1;
        initialized = true;
    }

    /* generate further terms until we have at least n of them */
    while (cache_size < n) {
        /* search for the next candidate that is 2,3,4 or 5 (mod 19) and prime */
        while (1) {
            idx_t r = next_candidate % 19U;
            if ((r == 2U || r == 3U || r == 4U || r == 5U) && is_prime(next_candidate)) {
                /* grow cache if required */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp)
                        return 0;                     /* allocation failed */
                    cache = tmp;
                }
                cache[cache_size++] = next_candidate;
                next_candidate += 2;                  /* advance to the next odd number */
                break;
            }
            next_candidate += 2;                      /* skip to the next odd number */
        }
    }

    return cache[n - 1];                          /* return the n‑th term (1‑based) */
}
