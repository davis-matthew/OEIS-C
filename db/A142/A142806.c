/*
 * A142806  Primes congruent to 8 mod 61.
 * https://oeis.org/A142806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 106, u32 n <= 107, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 8 mod 61
   OEIS A142806
   a(n) = the n-th prime p such that p ≡ 8 (mod 61),  n ≥ 1
*/
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static storage for already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t  next_candidate = 0;
    static bool initialized = false;

    /* Initialize on first call */
    if (!initialized) {
        initialized = true;
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size  = 0;
        /* First prime with p ≡ 8 (mod 61) is 191 = 8 + 61*3 */
        next_candidate = (seq_t)8 + (seq_t)61 * (seq_t)3;
    }

    /* Generate enough terms to answer this request */
    while (cache_size < n) {
        /* Find next prime in the arithmetic progression 8 (mod 61) */
        while (!is_prime_big(next_candidate))
            next_candidate += (seq_t)61;

        /* Store the found prime */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = next_candidate;
        next_candidate += (seq_t)61;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
