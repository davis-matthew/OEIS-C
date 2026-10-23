/*
 * A142700  Primes congruent to 56 mod 57.
 * https://oeis.org/A142700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 56 (mod 57)
 * Range:     u8 n <= 2, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A142700, offset = 1)                                   */

#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms (0‑based) */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  sz    = 0;
    /* current multiplier k for the form 57 k + 56   */
    static idx_t   k     = 0;

    if (n == 0)
        return 0;                    /* unused, offset is 1 */

    /* Grow the array until it holds at least n terms */
    while (sz < (size_t)n)
    {
        /* Allocate or expand the cache as needed */
        if (cap <= sz)
        {
            size_t newcap = cap ? cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                return 0;            /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* Candidate of the form 57*k + 56 */
        seq_t cand = (seq_t)57 * k + (seq_t)56;

        /* Test primality */
        if (cand >= 2 && is_prime_small(cand))
        {
            cache[sz++] = cand;
        }
        /* Advance to the next multiplier for both primes and composites */
        ++k;
    }

    return cache[n - 1];
}
