/*
 * A061668  Numbers k such that x-4, x-2, x+2, x+4 are primes, where x = 30*k
 *          - 15.
 * https://oeis.org/A061668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that 30*k-19, 30*k-17, 30*k-13, 30*k-11 are all prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached k values */
    static idx_t cache_len = 0;   /* number of cached k's */
    static idx_t cache_cap = 0;   /* capacity of cache array */
    static idx_t next_k = 1;      /* next k to test */

    /* initialize the cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* extend the cache until it contains at least n values */
    while (cache_len < n) {
        seq_t val1 = (seq_t)30 * next_k - 19;
        seq_t val2 = (seq_t)30 * next_k - 17;
        seq_t val3 = (seq_t)30 * next_k - 13;
        seq_t val4 = (seq_t)30 * next_k - 11;

        bool ok = is_prime((idx_t)val1) &&
                  is_prime((idx_t)val2) &&
                  is_prime((idx_t)val3) &&
                  is_prime((idx_t)val4);

        if (ok) {
            /* grow the array if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
