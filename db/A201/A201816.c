/*
 * A201816  Numbers k such that 90*k + 13 is prime.
 * https://oeis.org/A201816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that 90*k + 13 is prime
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine implements the sequence A201816.  It keeps a dynamic
   cache of the computed k‑values; when a larger index is requested
   the cache is extended and new candidates are tested for primality.
   The primality test is supplied by the build system in
   "../utils/is_prime.h". */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* make the primality test available */
#include "../utils/is_prime.h"

/* Forward declaration */
static void grow_cache(size_t min_cap);

/* Internal cache of the sequence values. */
static seq_t *cache   = NULL;  /* array of computed k values */
static size_t cache_cap = 0;   /* total allocated slots */
static size_t cache_used = 0;  /* filled slots in cache */

/* The next candidate k to be examined.  The cache is filled up to
   n entries, then next_k continues from where it left off. */
static uint64_t next_k = 0;

/* Ensure that at least min_cap slots are available in the cache. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap < min_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) abort();          /* undefined for n < 1 */

    /* Initialise on first call. */
    if (cache_used == 0)
    {
        grow_cache(64);
        cache[0] = (seq_t)0;    /* a(1) = 0  → 90*0+13 = 13 (prime) */
        cache_used = 1;
        next_k = 1;             /* start testing from k = 1 */
    }

    /* Generate terms until we have at least n entries. */
    while (cache_used < (size_t)n)
    {
        /* Compute 90*k + 13; we stay in 64‑bit arithmetic while searching. */
        uint64_t candidate = 90ULL * next_k + 13ULL;
        if (is_prime(candidate))
        {
            grow_cache(cache_used + 1);
            cache[cache_used++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
