/*
 * A005846  Primes of the form k^2 + k + 41.
 * https://oeis.org/A005846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th prime of the form k^2 + k + 41, k = 0, 1, 2, ... */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_k = 0;

    if (n <= cache_len) {
        return (seq_t)cache[n - 1];
    }

    if (cache == NULL) {
        cache_cap = 1024;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
        if (!cache) return (seq_t)0;
    }

    while (cache_len < n) {
        uint64_t c = next_k * next_k + next_k + 41;
        if (is_prime(c)) {
            if (cache_len >= cache_cap) {
                idx_t new_cap = cache_cap * 2;
                uint64_t *nc = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!nc) return (seq_t)0;
                cache = nc;
                cache_cap = new_cap;
            }
            cache[cache_len++] = c;
        }
        next_k++;
    }

    return (seq_t)cache[n - 1];
}
