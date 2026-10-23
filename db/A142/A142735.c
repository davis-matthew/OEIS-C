/*
 * A142735  Primes congruent to 8 mod 59.
 * https://oeis.org/A142735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 8 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
    static seq_t next_candidate = 0;

    if (n == 0) return 0;  /* undefined for n < 1, return 0 as placeholder */

    /* lazy initialization of cache and first candidate */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_candidate = (seq_t)67;   /* first prime = 8 + 59*1 */
    }

    /* extend cache until we have at least n elements */
    while (cache_count < (size_t)n)
    {
        if (is_prime_big(next_candidate))
        {
            if (cache_count == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = next_candidate;
        }
        next_candidate += (seq_t)59;  /* advance to next number ≡ 8 (mod 59) */
    }

    return cache[n - 1];
}
