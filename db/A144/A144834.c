/*
 * A144834  Numbers n such that the two numbers n+1 and n+3 are both prime.
 * https://oeis.org/A144834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 860, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A144834: numbers n such that n+1 and n+3 are both prime
 * (i.e. n = p−1 for each twin prime pair p, p+2).
 * The function generates twin primes on demand and caches results.
 * It works for all idx_t >= 1 and is independent of call order.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of terms stored */
    static idx_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_candidate = 2;/* next n to test */

    /* Return cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache as needed */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Find twin primes until we have at least n terms */
    while (cache_len < n) {
        /* Test if next_candidate + 1 and + 3 are both prime */
        if (isPrimeGeneric((seq_t)(next_candidate + 1)) &&
            isPrimeGeneric((seq_t)(next_candidate + 3))) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
