/*
 * A215135  Primes congruent to {1, 2, 3} mod 13.
 * https://oeis.org/A215135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 1, 2, or 3 (mod 13)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* Static cache of primes found so far. */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;        /* number of primes in cache       */
    static uint64_t last_checked = 1;    /* largest integer already examined */

    /* Extend the cache until we have at least n primes. */
    while (cache_len < n) {
        uint64_t candidate = last_checked + 1;   /* next number to test     */

        /* Scan forward until a suitable prime is found. */
        for (;;) {
            uint64_t r = candidate % 13;
            if (!(r == 1 || r == 2 || r == 3)) {
                /* not an allowed residue, skip */
                ++candidate;
                continue;
            }

            /* Test for primality. */
            if (isPrimeTrial((seq_t)candidate)) {
                /* Grow the cache for the new entry. */
                seq_t *tmp = (seq_t *)realloc(cache,
                                               (cache_len + 1) * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – signal an error through a
                       suitable return.  The caller is not expected to
                       handle this, but we guard against UB. */
                    return 0;
                }
                cache = tmp;
                cache[cache_len++] = (seq_t)candidate;
                last_checked = candidate;
                break; /* prime added – go back to the outer loop */
            }

            ++candidate;   /* try the next integer */
        }
    }

    return cache[n - 1];
}
