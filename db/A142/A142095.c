/*
 * A142095  Primes congruent to 29 mod 35.
 * https://oeis.org/A142095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 947 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 29 (mod 35).  The function returns the nth term
   of OEIS A142095, with offset 1.  The implementation keeps a cache
   of already-queried primes and extends it as needed.  Candidates are
   generated as 29, 64, 99, ... (i.e. 29 + 35*k) and accepted if
   they are prime using isPrimeGeneric. */

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
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of values stored */
    static idx_t cache_capacity = 0; /* current allocation size */
    static seq_t next_candidate;     /* next number to test */
    static bool initialized = false; /* ensure one-time init */

    if (n == 0) return 0; /* offset is 1, but caller should not request n=0 */

    /* Return cached value if already computed */
    if (cache && n <= cache_size)
        return cache[n - 1];

    /* Initialisation */
    if (!initialized) {
        cache_capacity = 64;
        cache = mem_new(seq_t, cache_capacity);
        if (!cache) abort(); /* allocation failure */
        cache_size = 0;
        next_candidate = 29;
        initialized = true;
    }

    /* Grow cache until we have at least n elements */
    while (cache_size < n) {
        /* Test current candidate */
        if (isPrimeGeneric(next_candidate)) {
            /* Ensure enough space */
            if (cache_size >= cache_capacity) {
                idx_t new_cap = cache_capacity * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        next_candidate += 35; /* next number in the 29 mod 35 progression */
    }

    return cache[n - 1];
}
