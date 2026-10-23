/*
 * A142603  Primes congruent to 3 mod 55.
 * https://oeis.org/A142603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 3 mod 55
 * Range:     u8 n <= 3, u16 n <= 150, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* a(1) is the first prime ≡ 3 (mod 55) */
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t candidate  = 3;   /* next odd number to test */

    while (cache_size < n)
    {
        if ( (candidate % (seq_t)55) == 3 && is_prime_big(candidate) )
        {
            if (cache_size == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure – fallback */
                cache = tmp;
            }
            cache[cache_size++] = candidate;
        }
        candidate += 2;   /* only odd numbers are needed */
    }

    return cache[n - 1];
}
