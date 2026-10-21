/*
 * A050268  Primes of the form 36*k^2 - 810*k + 2753, listed in order of
 *          increasing parameter k >= 0.
 * https://oeis.org/A050268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 4313, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050268 – primes of the form 36*k*k - 810*k + 2753, listing
 * the nth prime in increasing order of the non‑negative integer k.
 * The function generates the sequence lazily, memoising results,
 * so repeated or out‑of‑order calls are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Cache for already found primes, the index of the first k that
     * has not yet been examined, and the size of the cache.  These
     * are static so that successive calls preserve state. */
    static seq_t *cache = NULL;   /* array of found primes */
    static size_t  cap   = 0;     /* current capacity of cache */
    static idx_t   count = 0;     /* number of primes stored */
    static idx_t   k     = 0;     /* next k to test */

    /* Keep walking k until we have found at least n primes. */
    while (count < n)
    {
        /* Compute the polynomial value for the current k.  All operations
         * are performed in type seq_t to avoid overflow of the result.
         * Casting k to seq_t promotes the arithmetic to the required
         * precision. */
        seq_t val = (seq_t)36 * (seq_t)k * (seq_t)k
                     - (seq_t)810 * (seq_t)k
                     + (seq_t)2753;

        /* A valid member of the sequence must be positive and prime. */
        if (val > 0 && isPrimeGeneric(val))
        {
            /* Grow the cache when needed.  We double the capacity
             * to keep the amortised cost of reallocations low.  The
             * cast to seq_t* is accepted by both C and C++ (C-style
             * cast performs a reinterpret_cast in C++). */
            if (cap <= count)
            {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = val;
        }

        /* Advance to the next candidate k. */
        ++k;
    }

    return cache[n - 1];
}
