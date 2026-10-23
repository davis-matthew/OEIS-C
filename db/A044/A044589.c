/*
 * A044589  Numbers n such that string 2,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains25_oct_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the *n*‑th integer that contains the substring 25
   in its base‑8 representation but whose successor does not. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains25_oct_idx.h"

/* ------------------------------------------------------------------ */
/* a(n) – the n‑th element of A044589 (offset 1)                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n==0 – return 0 */

    /* Static cache of already generated terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_search = 0;  /* next integer to test */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        /* Use a C‑style cast – required for both C and C++ */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least *n* of them */
    while (cache_len < n) {
        /* Find the next integer with the required property */
        idx_t cand = next_search;
        for (;;) {
            if (contains25_oct_idx(cand) && !contains25_oct_idx(cand + 1))
                break;
            ++cand;
        }
        next_search = cand + 1;    /* next search starts after this cand */

        /* Grow cache if needed */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Store the candidate, letting the cast create a seq_t value
           from the integral type when necessary. */
        cache[cache_len] = (seq_t)cand;
        ++cache_len;
    }

    /* Return the requested element (offset 1 → index n‑1) */
    return cache[n - 1];
}
