/*
 * A108029  Numbers k such that 149*k + 151 is prime.
 * https://oeis.org/A108029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 4397, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9342 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108029: numbers k such that 149*k + 151 is prime.
   Computation: generate k sequentially, test 149*k+151 for primality,
   and store the k-values so later calls can reuse earlier results. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache for already found terms */
    static seq_t *cache = NULL;        /* array of k-values */
    static idx_t cached = 0;           /* number of cached terms */
    static size_t cap = 0;             /* allocated capacity of cache */
    static uint64_t last_k = 0;        /* next k to test (0-based) */

    /* Ensure the cache can hold the requested term */
    if (n == 0) {
        /* Undefined for n < 1, but we simply return 0. */
        return (seq_t)0;
    }

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        cap = 1024;
        cache = mem_new(seq_t, cap);
    }
    while (cached < n) {
        /* Grow cache when full */
        if (cached >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = mem_new(seq_t, new_cap);
            mem_copy(tmp, cache, cached * sizeof(seq_t));
            mem_free(cache);
            cache = tmp;
            cap = new_cap;
        }

        /* Test current k */
        uint64_t candidate = (149ULL * last_k) + 151ULL;
        if (is_prime_u64(candidate)) {
            cache[cached++] = (seq_t)last_k;
        }
        last_k++;
    }

    return cache[n - 1];
}
