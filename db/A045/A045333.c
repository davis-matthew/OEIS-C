/*
 * A045333  Primes congruent to {0, 2, 3} (mod 7).
 * https://oeis.org/A045333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 0, 2, or 3 (mod 7) (OEIS A045333)
   The function scans integers incrementally, testing primality with
   is_prime_big(seq_t).  Primes that satisfy p≡0,2,3 (mod 7) are cached
   and returned in order.  The cache is preserved between calls. */

/* Includes – adjust paths for the build system if necessary */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes and current candidate */
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;
    static size_t cache_cap  = 0;
    static uint64_t cand     = 2;   /* next integer to test */

    /* fill cache until it contains at least n primes */
    while (cache_cnt < n)
    {
        /* grow cache if needed */
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cnt == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* search for the next prime that satisfies the congruence */
        while (true)
        {
            seq_t val = (seq_t)cand;
            if (is_prime_big(val) &&
                (cand == 7 || cand % 7 == 2 || cand % 7 == 3))
            {
                /* store prime and advance candidate for future searches */
                cache[cache_cnt++] = val;
                cand++;
                break;
            }
            cand++;   /* skip non‑matching value */
        }
    }

    return cache[n - 1];
}
