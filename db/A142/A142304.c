/*
 * A142304  Primes congruent to 27 mod 44.
 * https://oeis.org/A142304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth prime congruent to 27 (mod 44)
 * Range:     u8 n <= 1, u16 n <= 321, u32 n <= 1000, u64 n <= 1000,
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
    /* undefined for offset 0 */
    if (n == 0) return 0;

    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* progress along the arithmetic progression 44*k+27 */
    static idx_t last_k   = 1;          /* k such that 44*k+27 = 71 */
    static idx_t last_prime = 71;       /* first prime in the sequence */

    /* initialize cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)71;
    }

    /* extend cache until it contains at least the nth term */
    while (cache_len < (size_t)n) {
        idx_t candidate;
        do {
            ++last_k;
            candidate = last_k * 44 + 27;
        } while (!is_prime(candidate));

        last_prime = candidate;

        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)last_prime;
    }

    return cache[n - 1];
}
