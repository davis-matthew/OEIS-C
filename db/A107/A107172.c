/*
 * A107172  Primes of the form 6x^2 + 11y^2.
 * https://oeis.org/A107172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A107172 Primes of the form 6*x^2 + 11*y^2 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* 64‑bit integer square root (truncated). */
static uint64_t isqrt_uint64(uint64_t n) {
    uint64_t low = 0, high = ((uint64_t)1 << 32);
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

/* Test whether a prime p can be written as 6x^2 + 11y^2. */
static bool representable_uint64(uint64_t p) {
    uint64_t max_y = isqrt_uint64(p / 11);
    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t y2  = y * y;
        uint64_t term = 11 * y2;
        if (term > p) break;           /* allow term == p        */
        uint64_t rem = p - term;
        if (rem % 6 != 0) continue;
        uint64_t x2 = rem / 6;
        uint64_t x = isqrt_uint64(x2);
        if (x * x == x2) return true;  /* perfect square found */
    }
    return false;
}

/* Cache of primes that satisfy the representation. */
static seq_t *cache      = NULL;
static idx_t cache_size  = 0;
static idx_t cache_cap   = 0;
static uint64_t next_candidate = 2;

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_size < n) {
        /* Adjust candidate to the next odd number if necessary. */
        if (next_candidate > 2 && next_candidate % 2 == 0) ++next_candidate;

        /* Find the next prime. */
        while (!is_prime(next_candidate))
            next_candidate += 2;          /* skip even numbers */

        /* Record it if it has the desired representation. */
        if (representable_uint64(next_candidate)) {
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }

        /* Proceed to the next odd candidate. */
        next_candidate += 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for index 0 */
    ensure_cache(n);
    return cache[n - 1];
}
