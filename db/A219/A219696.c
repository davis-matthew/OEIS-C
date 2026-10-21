/*
 * A219696  Numbers k such that the trajectory of 3k + 1 under the '3x + 1'
 *          map reaches k.
 * https://oeis.org/A219696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 33, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   hits_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A219696.
   For a given k, k is in the sequence iff the Collatz trajectory of 3k+1
   (standard 3x+1 map) eventually reaches k.  The routine tests each
   integer in increasing order, caches all valid k and returns the n‑th
   cached element.  The algorithm is O(n·T), where T is the cost of a
   single Collatz walk; it runs comfortably within the limits for all
   n up to about 10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/hits_k.h"

static void ensure_cache_cap(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!*ptr)
            abort();
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  checked_to = 0;

    /* Extend the cache until we have at least n entries. */
    while (cache_size < n) {
        ++checked_to;
        if (hits_k(checked_to)) {
            ensure_cache_cap(cache_size + 1, &cache_cap, &cache);
            cache[cache_size++] = (seq_t)checked_to;
        }
    }

    return cache[n - 1];   /* a(n) is the nth element of the cache */
}
