/*
 * A043224  Numbers k such that 0 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_09
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043224: numbers k such that the decimal expansion of k
   contains the adjacent pair "09" or "90", but the decimal expansion
   of k-1 does not.  The function a(n) returns the n‑th such number
   (1‑based index).  It is computed by a simple scan with memoisation
   and runs in O(n) time with a negligible constant factor. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_09.h"

/* Cache storage for previously found terms. */
static seq_t *cache      = NULL;    /* array of found terms */
static idx_t  cache_size = 0;       /* number of terms currently stored */
static idx_t  cache_cap  = 0;       /* allocated capacity of cache */
static idx_t  last_checked = 0;     /* last integer that was examined */

/* Ensure that the cache has capacity for at least 'min_cap' terms. */
static void ensure_cache_capacity(idx_t min_cap)
{
    if (cache_cap < min_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new term to the cache, growing storage if necessary. */
static void append_to_cache(seq_t value)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = value;
}

/* Compute the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;  /* undefined for n=0; return 0 as sentinel */

    /* Grow cache until it contains at least n terms. */
    while (cache_size < n) {
        ++last_checked;  /* examine next integer */
        if (has_adjacent_09(last_checked) &&
            !has_adjacent_09(last_checked - (seq_t)1))
            append_to_cache(last_checked);
    }
    return cache[n - 1];
}
