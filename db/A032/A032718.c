/*
 * A032718  Numbers k such that k prefixed by '1' and followed by '7' is
 *          prime.
 * https://oeis.org/A032718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032718: Numbers k such that the decimal integer 1k7 is prime. */
/* The function returns the n‑th such k, with a(1)=0.  It is computed
   by incrementally testing successive k and using the helper
   isPrimeGeneric() for primality testing.  Results are cached to
   provide O(1) lookup for later calls.  The implementation works
   for any seq_t (up to 128‑bit and arbitrary precision). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* ------------------------------------------------------------------ */
/* Cached sequence values                                             */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* array of found k's              */
static size_t cache_size = 0;      /* number of elements stored       */
static size_t cache_cap  = 0;      /* allocated capacity of cache     */

/* ------------------------------------------------------------------ */
/* Iterator state for generating new terms                            */
/* ------------------------------------------------------------------ */
static seq_t next_k          = 0;   /* next k to test                  */
static size_t len            = 1;   /* number of decimal digits in k   */
static seq_t pow_len10       = 10;  /* 10^len                          */
static seq_t pow_lenPlus1    = 100; /* 10^(len+1)                       */

/* ------------------------------------------------------------------ */
/* Ensure that the cache has at least `new_cap` capacity              */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;               /* silently ignore out‑of‑memory */
    cache   = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Some callers may pass 0; undefined in OEIS, but return 0 for safety. */
    if (n == 0)
        return 0;

    /* Initialise static storage on first call. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;
        next_k   = 0;
        len      = 1;
        pow_len10  = 10;
        pow_lenPlus1 = 100;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Compute candidate number 1k7. */
        seq_t candidate = pow_lenPlus1 + next_k * 10 + 7;

        /* Test primality and, if prime, store k. */
        if (isPrimeGeneric(candidate)) {
            /* Grow cache if necessary. */
            if (cache_size == cache_cap)
                ensure_cache_capacity(cache_cap * 2);
            if (cache && cache_size < cache_cap) {
                cache[cache_size++] = next_k;
            }
        }

        /* Advance to next k. */
        ++next_k;

        /* Update digit‑length bookkeeping when a new digit is reached. */
        if (next_k == pow_len10) {          /* e.g. 9 → 10, 99 → 100 */
            pow_len10   *= 10;              /* now 10^(len+1)             */
            pow_lenPlus1 = pow_len10 * 10;  /* now 10^(len+2)             */
            ++len;
        }
    }

    return cache[(size_t)n - 1];
}
