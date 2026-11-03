/*
 * A214886  Primes of the form n^3-2*n^2-1.
 * https://oeis.org/A214886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form k^3-2*k^2-1 (k>0)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 48 terms through n = 893 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *prime_cache = NULL; /* cache of computed primes   */
static size_t cache_cap   = 0;     /* capacity of the cache      */
static size_t cache_len   = 0;     /* number of primes cached    */
static idx_t last_k       = 4;     /* next k to test (start at 4) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for bad input         */

    /* Grow the cache until we have at least n primes */
    while (cache_len < n) {
        seq_t k  = (seq_t)last_k;
        seq_t val = k * k * k - (seq_t)2 * k * k - (seq_t)1;
        if (isPrimeGeneric(val)) { /* test primality  */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 32;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_len++] = val;
        }
        ++last_k;
    }
    return prime_cache[n-1];        /* return the requested prime  */
}
