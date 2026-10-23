/*
 * A141954  Primes congruent to 10 mod 27.
 * https://oeis.org/A141954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 282, u32 n <= 327, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141954: primes p such that p ≡ 10 (mod 27)
   (computed by primality test and caching) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t cand = 0;
    static bool initialized = false;

    if (!initialized) {
        cand = 37;              /* first prime ≡ 10 mod 27 */
        initialized = true;
    }

    if (n == 0)
        return 0;               /* undefined for index 0, but safe */

    /* Ensure cache contains at least n elements */
    if (cache_len < n) {
        size_t need = (size_t)n;
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap < need) {
            while (cache_cap < need) cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        while (cache_len < n) {
            if (is_prime_big(cand))
                cache[cache_len++] = cand;
            cand += 27;
        }
    }

    return cache[n - 1];
}
