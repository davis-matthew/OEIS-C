/*
 * A166547  Primes of the form 100*k+7.
 * https://oeis.org/A166547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Sequence: nth prime of the form 100*k+7 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current capacity of the cache      */
    static size_t len   = 0;   /* number of primes stored            */
    static idx_t  k      = 0;   /* current k to test (candidate = 100*k+7) */

    if (cache == NULL) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
    }

    /* generate primes until we have n of them */
    while (len < n) {
        seq_t cand = (seq_t)100 * (seq_t)k + (seq_t)7;  /* 100*k + 7 */
        if (isPrimeGeneric(cand)) {
            if (len == cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = cand;
        }
        ++k;
    }

    return cache[(size_t)(n - 1)];
}
