/*
 * A045121  Numbers whose base-4 representation contains exactly two 1's and
 *          two 3's.
 * https://oeis.org/A045121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   ok_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045121: numbers whose base‑4 representation contains exactly two 1’s and two 3’s.
   The function searches for the next suitable integer whenever more terms are needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ok_base4.h"

/* Cached terms of the sequence */
static seq_t *cache  = NULL;
static size_t cache_size = 0;   /* number of elements currently stored */
static size_t cache_cap   = 0;   /* allocated capacity */

/* The value of the last term found (as a seq_t so it can hold arbitrarily large
   numbers if seq_t is an arbitrary‑precision type). */
static seq_t last_found = 0;

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n)
    {
        seq_t candidate = last_found + 1;   /* start searching from the next integer */
        while (!ok_base4(candidate))
            candidate = candidate + 1;     /* advance to the next candidate */

        /* Append the found candidate to the cache */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap ? 2 * cache_cap : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort(); /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
        last_found = candidate;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for offset < 1, but return 0 harmlessly */

    ensure_cache(n);
    return cache[n - 1];
}
