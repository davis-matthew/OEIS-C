/*
 * A100420  Numbers n such that 30*n+{1,7,11,17,19,23,29} are all prime.
 * https://oeis.org/A100420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 30*n + {1,7,11,17,19,23,29} are all prime.
   Computation is by iterative search, testing each candidate ≡4 (mod 7)
   with the eight primality checks.  Caching is used so that calls
   with arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/*  Cache of previously found terms                                    */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* values a(1), a(2), ...             */
static idx_t cache_len = 0;      /* number of terms stored              */
static idx_t cache_cap = 0;      /* current capacity of the array       */

/* Ensure the cache has room for at least "need" elements. */
static void ensure_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    while (cache_cap < need)
        cache_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Append a new term to the cache. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
        ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}

/* Test whether a candidate n gives seven consecutive primes. */
static bool valid_candidate(idx_t n)
{
    static const unsigned int offs[7] = {1, 7, 11, 17, 19, 23, 29};

    /* 30*n fits in 64 bits for all terms needed here; use it directly. */
    unsigned long long base = 30ULL * (unsigned long long)n;

    for (int i = 0; i < 7; ++i) {
        unsigned long long x = base + offs[i];
        if (!is_prime(x))        /* is_prime is defined in utils/is_prime.h */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Undefined for 0; return 0 for consistency, though offset is 1. */
    if (n == 0)
        return 0;

    /* Static variable to remember the smallest candidate next to test. */
    static idx_t next_candidate = 4;   /* first integer ≡4 (mod 7) */

    /* Generate new terms until we have at least n entries. */
    while (cache_len < n) {
        idx_t cand = next_candidate;

        /* Skip candidates until one passes all primality tests. */
        while (!valid_candidate(cand))
            cand += 7;

        /* Store the found term and update the next candidate. */
        cache_push((seq_t)cand);
        next_candidate = cand + 7;
    }

    return cache[n - 1];
}
