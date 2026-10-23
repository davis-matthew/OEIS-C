/*
 * A119046  Numbers k such that the k-th triangular number contains only
 *          digits {0,1,8}.
 * https://oeis.org/A119046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   good_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A119046: k-th integer such that the k-th triangular
   number contains only the digits 0, 1 and 8. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/good_triangular.h"

/* Persistent cache of discovered k values. */
static seq_t *cache = NULL;          /* array of values */
static size_t cache_size = 0;        /* number of values stored */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t  last_checked = 0;      /* last k examined */

/* Grow the cache array when more space is needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least n terms (1‑based index). */
static void ensure_cache_size(idx_t n)
{
    if (n <= 0 || cache_size >= (size_t)n)
        return;
    if (!cache)
        grow_cache();

    while (cache_size < (size_t)n) {
        ++last_checked;                    /* next integer to test */
        if (good_triangular(last_checked)) {
            if (cache_size >= cache_cap)
                grow_cache();
            cache[cache_size++] = (seq_t)last_checked; /* store the found k */
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* undefined for n<1; guard */
    ensure_cache_size(n);                /* build cache if needed */
    return cache[n - 1];                  /* 0‑based array */
}
