/*
 * A045373  Primes congruent to {0, 1, 2, 4} mod 7.
 * https://oeis.org/A045373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045373: a(n) = the n‑th prime p with p ≡ 0,1,2,4 (mod 7) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Return the n‑th prime congruent to 0,1,2 or 4 modulo 7.  */
seq_t a(idx_t n)
{
    /* cache of computed sequence values */
    static seq_t *cache  = NULL;
    static size_t cacheSize = 0;
    static size_t cacheCap  = 0;

    /* value to test next */
    static uint64_t current = 2;

    /* already computed? */
    if (cache && n <= cacheSize)
        return cache[n - 1];

    /* initialise cache */
    if (!cacheCap) {
        cacheCap = 16;
        cache = mem_new(seq_t, cacheCap);
    }

    /* generate terms until we have n of them */
    while (cacheSize < n) {
        uint64_t cand = current++;
        uint64_t r = cand % 7ULL;
        if (r == 0 || r == 1 || r == 2 || r == 4) {
            if (isPrimeGeneric((seq_t)cand)) {
                /* grow cache if necessary */
                if (cacheSize == cacheCap) {
                    cacheCap *= 2;
                    cache = mem_resize(seq_t, cache, cacheCap);
                }
                cache[cacheSize++] = (seq_t)cand;
            }
        }
    }

    return cache[n - 1];
}
