/*
 * A138220  Numbers k such that 900*k^2 + 1 is prime.
 * https://oeis.org/A138220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that 900*k^2+1 is prime */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t last_k = 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    while (cache_size < n) {
        ++last_k;
        uint64_t val = 900ULL * (uint64_t)last_k * (uint64_t)last_k + 1ULL;
        if (is_prime(val)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
