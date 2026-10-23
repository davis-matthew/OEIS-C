/*
 * A164290  Sequence of twin prime p where the middle term p+1 has 6 prime
 *          factors (here p+2 is the associated twin prime, not listed).
 * https://oeis.org/A164290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 153, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count_omega_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A164290:  Twin prime p such that p+1 has 6 prime
    factors (with multiplicity).  The function returns the n‑th such
    p   (offset 1).  It searches forward using the helper routine
    is_prime (declared in "../utils/is_prime.h") and counts prime
    factors of p+1 by trial division.  A static cache stores found
    values so that calls in any order are fast. */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_omega_uint64.h"

/* Static cache of discovered terms (stored as uint64_t).
   The cache is expanded lazily until it contains at least n terms. */
static uint64_t *cache_u64 = NULL;   /* allocated array */
static size_t   cache_cap  = 0;     /* current allocated capacity   */
static size_t   cache_len  = 0;     /* number of terms stored      */
static uint64_t next_candidate = 3; /* next odd number to test      */

/* Grow the cache to hold at least "needed" entries. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache_u64, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
    cache_u64 = tmp;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least n terms. */
static void extend_cache(idx_t n)
{
    /* Ensure enough storage first. */
    ensure_cache_cap((size_t)n);

    /* Continue searching until we have enough terms. */
    while (cache_len < (size_t)n)
    {
        /* Test whether next_candidate is the first of a twin‑prime pair. */
        if (is_prime((idx_t)next_candidate))
        {
            idx_t p2 = next_candidate + 2;
            if (is_prime(p2))
            {
                uint64_t mid = next_candidate + 1;
                if (count_omega_uint64(mid) == 6)
                {
                    /* Store the candidate in the cache. */
                    cache_u64[cache_len++] = next_candidate;
                    /* Resize if we reached current allocation. */
                    if (cache_len == cache_cap)
                        ensure_cache_cap(cache_cap * 2);
                }
            }
        }
        /* Advance to the next odd number. */
        next_candidate += 2;
    }
}

/* Public sequence function.  It guarantees O(1) time after the first
   call for any given n, with total time O(total_terms) over a batch
   of calls. */
seq_t a(idx_t n)
{
    assert(n > 0);                 /* sequence is defined for n >= 1 */
    extend_cache(n);
    return (seq_t)cache_u64[n - 1];
}
