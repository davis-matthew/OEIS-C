/*
 * A199369  Primes of the form 4*n^3 - 3.
 * https://oeis.org/A199369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 4*k^3-3 (searching over
 *            integer k)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 88, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 500 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for 0, return 0 */

    /* static cache of found primes and last tested k */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t  last_k = 0;    /* last value of k tested */

    /* if cache is empty, allocate initial capacity */
    if (cache == NULL) {
        /* start with capacity for 16 primes */
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        /* test next candidate k */
        last_k++;
        idx_t k = last_k;
        seq_t kk = (seq_t)k;
        seq_t val = 4 * kk * kk * kk - 3;   /* 4*k^3 - 3 */

        if (isPrimeGeneric(val)) {
            /* ensure capacity for new element */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = val;
        }
    }

    return cache[n - 1];
}
