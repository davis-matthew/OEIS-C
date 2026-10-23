/*
 * A142430  Primes congruent to 19 mod 49.
 * https://oeis.org/A142430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 19 (mod 49), offset 1 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of known terms          */
    static size_t cache_len = 0;         /* number of terms stored        */
    static size_t cache_cap = 0;         /* current capacity of cache     */
    static uint64_t last_prime_checked = 0; /* last prime examined          */

    /* initialise cache with the first term */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)19U;
        cache_len = 1;
        last_prime_checked = 19U;
    }

    /* generate terms until we have the requested one */
    while (cache_len < n) {
        uint64_t p = next_prime(last_prime_checked + 1U);
        last_prime_checked = p;
        if ((p % 49U) == 19U) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1U];
}
