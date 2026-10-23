/*
 * A028876  Numbers k such that k^2 - 5 is prime.
 * https://oeis.org/A028876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028876: Numbers k such that k^2 - 5 is prime.
   The term a(n) is obtained by scanning integers k starting from 4
   and inserting k into the sequence whenever k^2 - 5 is prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Cache of already discovered terms. */
static seq_t *cache   = NULL;   /* dynamic array of terms        */
static size_t cache_size = 0;   /* number of stored terms        */
static size_t cache_cap   = 0;   /* allocated capacity            */
static idx_t  next_k      = 4;   /* next integer to test          */

/* Grow the cache to hold at least ‘needed’ entries.               */
/* The cast from void* to seq_t* is required for C++ compatibility. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = (cache_cap == 0) ? 4 : (cache_cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }           /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A028876 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Compute k^2 - 5 using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 sq = (unsigned __int128)next_k * (unsigned __int128)next_k;
        seq_t trial = (seq_t)(sq - 5);

        if (isPrimeGeneric(trial)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
