/*
 * A023264  Primes that remain prime through 2 iterations of function f(x) =
 *          8x + 9.
 * https://oeis.org/A023264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 8888 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023264: prime p such that 8p+9 and 64p+81 are also primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* static state for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_candidate = 3;   /* first odd prime to test */

    while (cache_size < n) {
        if (isPrimeGeneric(next_candidate)) {
            seq_t v1 = (seq_t)8 * next_candidate + (seq_t)9;
            if (isPrimeGeneric(v1)) {
                seq_t v2 = (seq_t)64 * next_candidate + (seq_t)81;
                if (isPrimeGeneric(v2)) {
                    /* ensure capacity */
                    if (cache_size == cache_cap) {
                        size_t old_cap = cache_cap;
                        cache_cap = cache_cap ? cache_cap * 2 : 8;
                        cache = seq_realloc(cache, old_cap, cache_cap);
                    }
                    cache[cache_size++] = next_candidate;
                }
            }
        }
        /* advance to next odd candidate */
        next_candidate += 2;
    }

    return cache[n - 1];
}
