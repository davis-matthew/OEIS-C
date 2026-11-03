/*
 * A217500  Primes of the form 2*n^2 + 74*n + 35.
 * https://oeis.org/A217500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 2518, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 53 terms through n = 2998 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217500:
   a(k) is the k‑th prime of the form 2*n^2 + 74*n + 35.
   This implementation iteratively tests successive n starting at 1
   and collects the primes in a static cache. */

#include <stddef.h>
#include <stdlib.h>
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

static seq_t *prime_cache = NULL;   /* array of found primes */
static size_t prime_cache_len = 0;   /* number of primes stored */
static size_t prime_cache_cap = 0;   /* allocated capacity */
static idx_t next_n = 1;             /* next n to test in the quadratic */

/* Ensure the cache has room for at least one more element. */
static void grow_cache(void)
{
    if (prime_cache_len == prime_cache_cap) {
        size_t new_cap = (prime_cache_cap == 0) ? 8 : prime_cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) {
            /* Out of memory – terminate the program. */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
}

/* Return the k‑th prime of the quadratic form. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* undefined for zero index */
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= prime_cache_len) {
        return prime_cache[(size_t)n - 1];
    }

    /* Extend the cache until we have the desired term. */
    while ((size_t)n > prime_cache_len) {
        seq_t nn = (seq_t)next_n;
        seq_t p  = 2 * nn * nn + 74 * nn + 35;  /* 2*n^2 + 74*n + 35 */
        if (isPrimeGeneric(p)) {
            grow_cache();
            prime_cache[prime_cache_len++] = p;
        }
        ++next_n;
    }

    return prime_cache[(size_t)n - 1];
}
