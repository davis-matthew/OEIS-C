/*
 * A090696  Numbers k such that k^2 - 11 is a prime.
 * https://oeis.org/A090696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 4308, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 72 terms through n = 4889 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: a(n) is the n/*th positive integer k such that k²‑11 is prime.
   Cached values are stored on the first call and reused on subsequent calls. */

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

static seq_t *cache          = NULL; /* array of already found k values */
static size_t cache_len      = 0;    /* number of terms stored                */
static size_t cache_cap      = 0;    /* current allocation size               */
static idx_t  last_checked   = 3;    /* k that was tested most recently;       */
                                     /* start at 3 so the first candidate is 4 */

/* Return the n-th term (n >= 1) of OEIS A090696. */
seq_t a(idx_t n)
{
    /* Ensure one-time cache allocation. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Extend the sequence until it contains at least n terms. */
    while (cache_len < n) {
        /* Next candidate k. */
        idx_t k = last_checked + 1;

        /* Compute k² – 11 using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)k * k;
        seq_t val = (seq_t)(prod - 11);

        /* Test primality of k²‑11.  isPrimeGeneric works for all seq_t sizes. */
        if (isPrimeGeneric(val)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)k;   /* store the new term     */
        }

        last_checked = k;  /* remember the most recent k tested */
    }

    return cache[n - 1]; /* terms are 1‑based */
}
