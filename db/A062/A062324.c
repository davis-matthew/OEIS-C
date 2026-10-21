/*
 * A062324  Primes p such that p^2 + 4 is also prime.
 * https://oeis.org/A062324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 774, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search primes p such that p^2 + 4 is also prime */

/* The function builds the sequence in increasing order and caches it
 * for future calls.  It works for any seq_t and idx_t type provided
 * by the build system.  Arbitrary‑precision arithmetic is handled
 * via the existing isPrimeGeneric routine. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache   = NULL;   /* cached sequence values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t next_candidate = 3;   /* next odd number to test */

/* Ensure that cache contains at least n terms (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Grow cache incrementally */
    while (cache_len < (size_t)n) {
        /* Scan odd numbers until a suitable prime is found */
        for (;;) {
            /* skip even numbers */
            if (next_candidate < 3)
                next_candidate = 3;

            /* test primality of the candidate */
            if (isPrimeGeneric((seq_t)next_candidate)) {
                /* compute q = p^2 + 4, avoiding overflow on 64‑bit seq_t */
                seq_t q;
                if (sizeof(seq_t) <= 8) {
                    unsigned __int128 s = (unsigned __int128)next_candidate;
                    q = (seq_t)(s * s + 4);
                } else {
                    seq_t tmp = (seq_t)next_candidate;
                    q = tmp * tmp + (seq_t)4;
                }

                /* test whether q is also prime */
                if (isPrimeGeneric(q)) {
                    /* store the candidate */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = (seq_t *)realloc((void *)cache,
                                                 cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)next_candidate;
                    /* advance to next odd number for future searches */
                    next_candidate += 2;
                    break;   /* proceed to fetch next cached value */
                }
            }

            /* candidate did not satisfy condition; try next odd number */
            next_candidate += 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of range, not used by tests */
    ensure_cache(n);
    return cache[n - 1];
}
