/*
 * A044419  Numbers n such that string 8,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains87
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n whose decimal representation contains "87" but n−1 does not */

/* This implementation uses a simple incremental search with memoisation.
   It works for any of the allowed seq_t types (built‑in unsigned or
   arbitrary‑precision) and for any non‑zero index n. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains87.h"

/* Check whether n itself contains "87" but n-1 does not. */
static bool valid_candidate(seq_t n)
{
    if (!contains87(n)) return false;
    seq_t prev = n - (seq_t)1;
    return !contains87(prev);
}

/* Return the n-th term of the sequence (n >= 1).  The function memoises
   previously computed terms in a static cache so that calls with arbitrary
   ordering of n remain efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 0, harmless */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Allocate the cache once. */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Return cached value if available. */
    if (n <= cache_len) return cache[n - 1];

    /* Continue searching from just after the last cached term. */
    seq_t cand = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < n)
    {
        if (valid_candidate(cand))
        {
            if (cache_len == cache_cap)
            {
                cache_cap <<= 1; /* double the capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        cand = cand + (seq_t)1;
    }

    return cache[n - 1];
}
