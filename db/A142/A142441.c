/*
 * A142441  Primes congruent to 32 mod 49.
 * https://oeis.org/A142441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental search, caching primes ≡ 32 mod 49.
   Uses is_prime_big() for arbitrary‐precision values.*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* static cache of discovered primes, stored as seq_t */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* next candidate of the form 32 + 49·k, stored as seq_t */
    static seq_t next_candidate = 32;

    /* generate primes until we have n of them */
    while (cache_len < n)
    {
        seq_t cand = next_candidate;
        /* advance candidate for the next loop */
        next_candidate += 49;

        if (is_prime_big(cand))
        {
            /* ensure capacity */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;     /* allocation failed */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[(size_t)(n - 1)];
}
