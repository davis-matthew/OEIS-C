/*
 * A043164  Numbers k such that 1 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k such that its base‑8
 *            representation * contains the adjacent digits 1 and 7 in either
 *            order, while k-1 does not
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes – only standard headers allowed */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has17.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Cache of already found terms.  The array grows as needed. */
static seq_t *cache      = NULL;  /* pointer to array of seq_t   */
static size_t cache_len  = 0;     /* number of cached terms      */
static size_t cache_cap  = 0;     /* allocated capacity          */
static idx_t  next_k     = 1;     /* smallest integer to test next */

/* Grow the cache array when it becomes full. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;   /* *2 */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                                 /* unrecoverable */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of the sequence, 1‑based (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* below offset – irrelevant */

    /* Compute terms until we have at least n of them. */
    while (cache_len < n) {
        /* Find the next integer k that fits the required pattern. */
        while (!has17(next_k) || has17(next_k - 1))
            ++next_k;

        /* Store the result in the cache. */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;   /* advance past the found term for the next search    */
    }

    return cache[n - 1];
}
