/*
 * A111083  Numbers k such that 3*k^2 + 64 is prime.
 * https://oeis.org/A111083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k such that 3*k^2 + 64 is prime
 * Range:     u8 n <= 47, u16 n <= 5247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9519 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_sz   = 0;
    static idx_t  next_k     = 1;          /* next candidate k to test */

    /* Ensure capacity for the requested index */
    if (cache_sz < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < (size_t)n) new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Search for new terms until we have at least n values */
        while (cache_sz < (size_t)n) {
            uint64_t k  = (uint64_t)next_k;
            uint64_t val = 3ULL * k * k + 64ULL;   /* 3*k^2 + 64 */
            if (is_prime_u64(val)) {
                cache[cache_sz++] = (seq_t)k;
            }
            next_k++;
        }
    }

    return cache[n - 1];
}
