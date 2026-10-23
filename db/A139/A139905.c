/*
 * A139905  Primes of the form 11x^2+23y^2.
 * https://oeis.org/A139905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 826, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 11*x^2 + 23*y^2
   by brute‑force search, primality testing and memoisation.
   a(n) returns the nth such prime (a(1)=11). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_cache.h"

/* Integer square root of a 64‑bit unsigned integer. */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrtl((long double)n);
    /* adjust upward if we went too low */
    while (r + 1 <= n / (r + 1)) ++r;
    /* adjust downward if we went too high */
    while (r * r > n) --r;
    return r;
}

/* Test whether a 64‑bit prime p can be written as 11*x^2 + 23*y^2. */
static bool representable_uint64(uint64_t p)
{
    if (p == 11u) return true;           /* the only case with y = 0 */
    if (p % 11u == 0u) return false;     /* otherwise 11 divides p, impossible */

    uint64_t max_y_sq = p / 23u;
    uint64_t max_y = isqrt_uint64(max_y_sq);

    for (uint64_t y = 0; y <= max_y; ++y) {
        uint64_t y2 = y * y;
        uint64_t rem = p - 23u * y2;
        if (rem % 11u != 0u) continue;
        uint64_t x2 = rem / 11u;
        uint64_t x = isqrt_uint64(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* Local cache of found primes (stored as seq_t). */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Return the nth prime of the required form (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive input */

    /* Grow cache until it contains the nth term. */
    while (cache_len < (size_t)n) {
        /* Find next prime greater than last_checked. */
        static uint64_t last_checked = 1u;
        uint64_t candidate = last_checked + 1u;
        /* Skip even numbers after 2. */
        if (candidate == 2u) {
            last_checked = candidate;
        } else {
            if (candidate % 2u == 0u) ++candidate;
            while (!is_prime((idx_t)candidate))
                candidate += 2u;
            last_checked = candidate;
        }

        /* If candidate satisfies the quadratic form, store it. */
        if (representable_uint64(candidate)) {
            ensure_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)candidate;
        }
    }
    return cache[n - 1];
}
