/*
 * A201172  Primes of the form 5n^3+2.
 * https://oeis.org/A201172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 56, u64 n <= 1700,
 *            u128 n <= 1700, big any n
 * Validated: standard, pass, 42 terms through n = 1661 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime of the form 5*k^3+2 (OEIS A201172). */
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *prime_cache = NULL;   /* array of found primes            */
static idx_t  cache_size  = 0;       /* number of primes stored          */
static idx_t  cache_cap   = 0;       /* allocated capacity in elements    */
static idx_t  last_k      = 0;       /* last candidate k processed       */

/* Ensure that the cache can hold at least 'needed' primes.   */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1U : 64U;
    while (new_cap < needed)
        new_cap <<= 1U;

    seq_t *tmp;
    if (cache_cap == 0) {
        tmp = mem_new(seq_t, new_cap);
    } else {
        tmp = mem_resize(seq_t, prime_cache, new_cap);
    }
    if (!tmp)
        abort();
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Append a newly found prime to the cache. */
static void append_prime(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    prime_cache[cache_size++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n == 0 */

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return prime_cache[n - 1];

    /* Determine the next candidate k to test. */
    idx_t k = (cache_size == 0) ? 0 : last_k + 1;

    while (cache_size < n) {
        seq_t val;
        if (k == 0) {                         /* first candidate: 2 */
            val = (seq_t)2;
        } else {
            /* Even k > 0 produce even numbers, skip. */
            if ((k & 1U) == 0) {
                ++k;
                continue;
            }
            seq_t kk = (seq_t)k;
            val = (seq_t)5 * kk * kk * kk + (seq_t)2;
        }

        if (isPrimeGeneric(val))
            append_prime(val);

        last_k = k;
        ++k;
    }

    return prime_cache[n - 1];
}
