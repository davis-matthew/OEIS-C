/*
 * A200812  Primes of the form 5*n^3-6.
 * https://oeis.org/A200812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 69, u64 n <= 1100, u128 n <= 1100,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 5*n^3 - 6 (OEIS A200812). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Returns a(n) for n >= 1; undefined for n == 0 (behaviour below the
   offset is irrelevant). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* caller may ignore base case */

    /* --- static cache for previously found primes --- */
    static idx_t   next_k  = 1;      /* next k to test */
    static seq_t * cache   = NULL;   /* array of found primes */
    static size_t  cap     = 0;      /* allocated capacity */
    static size_t  len     = 0;      /* current number of primes stored */

    /* Grow the cache until we have at least n primes. */
    while (len < n) {
        seq_t k = (seq_t)next_k++;           /* current k value */

        /* Compute 5*k^3 - 6 safely in seq_t. */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        seq_t val = 5 * k3;
        if (val <= 6) continue;      /* skip (val - 6) would underflow */
        val -= 6;                    /* val = 5*k^3 - 6 */

        /* Test primality.  isPrimeGeneric works for all seq_t sizes. */
        if (isPrimeGeneric(val)) {
            /* If the cache is full, grow it (double the size). */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                /* cast is needed for C++ compatibility when seq_t is a class. */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = val;
        }
    }
    return cache[n - 1];
}
