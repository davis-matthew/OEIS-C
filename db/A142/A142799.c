/*
 * A142799  Primes congruent to 59 mod 60.
 * https://oeis.org/A142799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 60*k + 59 (OEIS A142799)
 * Range:     u8 n <= 3, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 855 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Returns the nth prime congruent to 59 mod 60, with n >= 1. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cached primes of this form   */
    static idx_t cache_cnt = 0;      /* number of cached primes      */
    static size_t cache_cap = 0;     /* current capacity of cache    */
    static seq_t next_candidate = 59;/* next number to test           */

    /* Grow the cache as needed and generate primes until we have n of them. */
    while (cache_cnt < n) {
        /* Allocate or expand the cache buffer. */
        if (cache_cap == 0) {
            cache_cap = 32;
            cache = mem_new(seq_t, cache_cap);
        } else if (cache_cnt >= cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        /* If next_candidate is prime, store it. */
        if (is_prime((idx_t)next_candidate)) {
            cache[cache_cnt++] = next_candidate;
        }

        /* Advance to the next number of the form 60*k + 59. */
        next_candidate += 60;
    }

    return cache[n - 1];
}
