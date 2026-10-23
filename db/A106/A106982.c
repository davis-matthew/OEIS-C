/*
 * A106982  Primes of the form 2x^2-xy+11y^2, with x and y nonnegative.
 * https://oeis.org/A106982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate primes of the form 2x^2 - xy + 11y^2 (x,y ≥ 0) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* is_prime(uint64_t n) is provided by the build utilities */
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/* integer square‑root for 64‑bit numbers (binary search) */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32; /* hi^2 > 2^64 */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------------------------------------------------ */
/* cache of computed terms */
static seq_t *cache = NULL;
static uint64_t cache_size = 0;
static uint64_t cache_cap = 0;
static uint64_t current_value = 2;   /* next integer to test */

/* helper: append a term to the cache, grow if necessary */
static void append_cache(seq_t val)
{
    if (cache_size >= cache_cap) {
        uint64_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ------------------------------------------------------------------ */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* generate terms until we have n entries */
    while (cache_size < (uint64_t)n) {
        uint64_t v = current_value++;
        if (!is_prime(v)) continue;

        /* check representability: find y with D = 8v - 87y^2 a perfect square */
        uint64_t y_max = isqrt_uint64((8 * v) / 87);
        bool ok = false;
        for (uint64_t y = 0; y <= y_max; ++y) {
            uint64_t D = 8 * v - 87 * y * y;
            if (D < 0) break;
            uint64_t s = isqrt_uint64(D);
            if (s * s != D) continue;
            /* x = (y ± s)/4 must be integer and nonnegative */
            if (((y + s) & 3ULL) == 0) {
                uint64_t x = (y + s) >> 2;
                if (x >= 0) { ok = true; break; }
            }
            if (((y >= s) && ((y - s) & 3ULL) == 0)) {
                uint64_t x = (y - s) >> 2;
                if (x >= 0) { ok = true; break; }
            }
        }
        if (ok) append_cache((seq_t)v);
    }

    return cache[n - 1];
}
