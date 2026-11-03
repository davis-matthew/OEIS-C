/*
 * A137770  Prime numbers p such that p +- ((p-1)/7) are primes.
 * https://oeis.org/A137770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that p ± (p-1)/7 are primes
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches primes in increasing order, testing only those
   congruent to 1 mod 7.  It keeps a static cache so that calls in any order
   are answered quickly.  The search stops as soon as _n_ terms have been found. */
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

static seq_t *cache      = NULL;  /* cached terms */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static seq_t next_candidate = 211; /* first number ≡ 1 (mod 7) that may satisfy */

/* Ensure the cache can hold at least 'needed' elements.  */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) return;           /* allocation failure – leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the OEIS offset is 1; behavior for n<1 is irrelevant */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        seq_t p = next_candidate;  /* start searching from the last candidate */
        while (true) {
            /* Only numbers ≡ 1 (mod 7) can possibly satisfy the condition. */
            if (isPrimeGeneric(p) && (p % 7 == 1)) {
                seq_t k = (p - 1) / 7;
                if (isPrimeGeneric(p - k) && isPrimeGeneric(p + k)) {
                    /* A new term has been found. */
                    grow_cache(cache_size + 1);
                    cache[cache_size++] = p;
                    next_candidate = p + 14;  /* next number ≡ 1 (mod 7) */
                    break;
                }
            }
            p = (seq_t)((unsigned long long)p + 14);  /* skip to next candidate */
        }
    }

    return cache[n - 1];
}
