/*
 * A049408  Numbers k such that k^4 + k + 1 is prime.
 * https://oeis.org/A049408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 895 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049408: numbers k such that k^4 + k + 1 is prime.
   This implementation computes the nth such k by incremental search
   and memoisation, suitable for arbitrary‑precision, 64‑bit, or
   smaller integer types. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_k     = 0;   /* last k found */

    /* ensure at least n terms are cached */
    while (cache_len < n) {
        /* find next k with k^4 + k + 1 prime */
        for (seq_t k = last_k + 1; ; ++k) {
            seq_t k2 = k * k;
            seq_t k4 = k2 * k2;
            seq_t val = k4 + k + 1;

            if (isPrimeGeneric(val)) {
                /* store k in cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();          /* allocation failure */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                last_k = k;
                break;   /* found next term */
            }
        }
    }

    return cache[(size_t)n - 1];
}
