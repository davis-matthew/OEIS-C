/*
 * A062931  Numbers k that, when expressed in base 5 and then interpreted in
 *          base 9, give a multiple of k.
 * https://oeis.org/A062931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   good_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062931: numbers k such that the base‑5 representation of k,
   interpreted as a base‑9 number, is a multiple of k. */
/* Implementation: iteratively test each integer k in increasing order
   until the requested n‑th term has been found.  The first term is
   a(1)=0. The sequence is stored in a static cache so that repeated
   calls with any n are answered in O(1) after the terms have been
   generated. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_k.h"

/* Forward declaration of the helper that checks the property for a
   64‑bit integer.  It is kept in the same translation unit because no
   external header for arbitrary‑precision integers is required. */
static bool good_k(uint64_t k);

/* ------------------------------------------------------------------
   static cache for sequence terms
   ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;    /* pointer to cache array      */
static size_t  cache_len  = 0;     /* number of terms cached      */
static size_t  cache_cap  = 0;     /* allocated capacity          */
static uint64_t next_test = 1;     /* next integer to test beyond
                                     the largest cached term      */

/* ------------------------------------------------------------------
   ensure that the cache has space for at least 'need' elements.
   ------------------------------------------------------------------ */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!newptr) abort();          /* allocation failure: terminate */
    seq_cache   = newptr;
    cache_cap   = newcap;
}

/* ------------------------------------------------------------------
   public sequence access function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* undefined, but safe */

    /* initialise cache with first term a(1)=0 */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        seq_cache[0] = 0;                 /* a(1) */
        cache_len    = 1;
        next_test    = 1;                 /* start testing from 1 */
    }

    /* if requested term already cached, return it */
    if ((size_t)n <= cache_len) {
        return seq_cache[n - 1];
    }

    /* generate terms until we reach the requested index */
    while (cache_len < (size_t)n) {
        /* find next integer that satisfies the property */
        while (!good_k(next_test)) {
            ++next_test;
        }
        /* store and advance */
        ensure_cache_cap(cache_len + 1);
        seq_cache[cache_len] = (seq_t)next_test;
        ++cache_len;
        ++next_test;                      /* next candidate starts after this */
    }

    return seq_cache[n - 1];
}
