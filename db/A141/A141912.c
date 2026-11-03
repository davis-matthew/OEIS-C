/*
 * A141912  Primes congruent to 8 mod 23.
 * https://oeis.org/A141912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 304, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 919 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime congruent to 8 (mod 23) computed by incremental search */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 31;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
        initialized = true;
    }

    while (cache_size < (size_t)n) {
        if (is_prime(next_candidate)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 23;
    }

    return cache[(size_t)n - 1];
}
