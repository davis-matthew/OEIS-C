/*
 * A044604  Numbers n such that string 4,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has44oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n ≥ 1 whose octal representation contains the
   substring 44 but the octal representation of n+1 does not. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has44oct.h"

/* Static cache for memoisation.  `cache_len`  = number of stored terms
   (i.e. the current index of the largest cached value).  `scan_last`
   is the largest integer that has already been tested for the property. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;     /* number of terms stored   */
static size_t cache_cap = 0;     /* allocated capacity       */
static seq_t  scan_last  = 0;    /* smallest integer NOT yet tested */

/* Main OEIS function.  The function is pure and thread‑safe
   except for the static cache.  It expands the cache until
   the requested term `n` (1‑based) is available. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for 0 is irrelevant */

    /* Grow cache until it contains at least `n` terms */
    while (cache_len < n) {
        /* Test next integer */
        seq_t m = ++scan_last;

        /* Check the defining property */
        if (has44oct(m) && !has44oct(m + (seq_t)1)) {
            /* Ensure enough space in the array */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();        /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;       /* store the found term */
        }
    }
    return cache[n - 1];   /* 0‑based index in the cache */
}
