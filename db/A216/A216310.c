/*
 * A216310  The prime ending in 3 is the only prime in a decade.
 * https://oeis.org/A216310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9861 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p = 10*k+3 such that 10*k+1, 10*k+7 and 10*k+9 are all
   composite.  The function builds a cache of the required primes and
   returns the n‑th one (n ≥ 1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;     /* array of found primes               */
static size_t cache_cap = 0;    /* allocated capacity (in seq_t units) */
static idx_t  cache_len = 0;    /* number of stored primes             */
static idx_t  next_k   = 0;     /* next k value to test                */

/* Ensure that cache has at least n elements. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Search for the next qualifying prime. */
        for (;;) {
            /* Candidate of the form 10*k+3. */
            seq_t cand = (seq_t)10 * (seq_t)next_k + (seq_t)3;

            /* Quick primality test on the candidate. */
            if (!isPrimeGeneric(cand)) {
                ++next_k;
                continue;
            }

            seq_t l1 = (seq_t)10 * (seq_t)next_k + (seq_t)1;
            seq_t l7 = (seq_t)10 * (seq_t)next_k + (seq_t)7;
            seq_t l9 = (seq_t)10 * (seq_t)next_k + (seq_t)9;

            /* All other numbers in the decade must be composite. */
            if (!isPrimeGeneric(l1) && !isPrimeGeneric(l7) && !isPrimeGeneric(l9)) {
                /* Grow cache if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) abort();              /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
                ++next_k;   /* advance k for the next search */
                break;      /* found the required prime */
            }
            ++next_k;      /* try next decade */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined for n < 1 – return 0 as a safe placeholder. */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[n - (idx_t)1];
}
