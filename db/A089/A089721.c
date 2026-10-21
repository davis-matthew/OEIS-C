/*
 * A089721  Primes ending in 1 such that floor(p/7) ends in a digit > 3.
 * https://oeis.org/A089721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089721
   Prime p such that p ≡ 1 (mod 10) and floor(p/7) ends in a digit > 3.
   This function searches primes on demand and caches results to support
   arbitrary call order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_candidate = 2;   /* start search from 2 */

    if (n == 0) {
        return (seq_t)0;   /* undefined for n = 0 */
    }

    if (cache_len >= n) {
        return cache[n - 1];
    }

    while (cache_len < n) {
        /* Find the next prime satisfying the condition. */
        uint64_t p = last_candidate;

        /* Align p to the next integer that ends with digit 1. */
        uint64_t rem = p % 10;
        if (rem != 1) {
            p += (1 + 10 - rem) % 10;
        }

        bool found = false;
        while (!found) {
            if (is_prime_small((seq_t)p)) {
                uint64_t f = p / 7;
                if ((f % 10) > 3) {
                    found = true;
                }
            }
            if (!found) {
                p += 10;   /* keep last digit 1 */
            }
        }

        /* Append the found prime to the cache. */
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);

        /* Next search will start after this prime, again ending with 1. */
        last_candidate = p + 10;
    }

    return cache[n - 1];
}
