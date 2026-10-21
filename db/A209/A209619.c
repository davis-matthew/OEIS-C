/*
 * A209619  Primes separated from their previous adjacent primes by a
 *          composite number of successive composites.
 * https://oeis.org/A209619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p whose previous adjacent prime pprev
 *            satisfies (p - pprev - 1) is a composite integer > 1
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t prev = 0;
    static uint64_t cur = 0;

    if (!initialized) {
        initialized = true;
        prev = 0;
        cur = next_prime(prev + 1);   /* start with the first prime, 2 */
    }

    /* Grow the cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t gap = cur - prev - 1;

        /* Check if the number of successive composites is composite */
        if (gap > 1 && !is_prime_big((seq_t)gap)) {
            /* Ensure buffer space */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* failure; unlikely in tests */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;
        }

        /* Advance to the next prime */
        prev = cur;
        cur = next_prime(cur + 1);
    }

    return cache[(size_t)n - 1];
}
