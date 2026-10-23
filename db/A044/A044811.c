/*
 * A044811  Numbers n such that string 9,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_98
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers n such that string 9,8 occurs in the base 10 representation
    of n but not of n+1.  The function returns the n-th such number
    (OEIS offset 1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_98.h"

/* Static cache of discovered terms.  All static variables are shared
   between calls.  */
static seq_t *cache   = NULL;   /* array of terms found so far */
static size_t cache_size = 0;   /* number of terms currently cached   */
static size_t cache_cap   = 0;   /* allocated size of the array        */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined behaviour for n = 0, harmlessly return 0 */
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_size) {
        return cache[(size_t)n - 1];
    }

    /* Grow the cache array if it does not yet accommodate the request. */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) {
            new_cap = needed;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            abort();   /* allocation failure – terminate */
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Search for the remaining terms.  Start from just after the last
       cached term (or from 1 if nothing is cached yet). */
    seq_t val = cache_size == 0 ? (seq_t)1 : cache[cache_size - 1] + (seq_t)1;

    while (cache_size < needed) {
        if (has_98(val) && !has_98(val + (seq_t)1)) {
            cache[cache_size++] = val;
        }
        val++;
    }

    return cache[(size_t)n - 1];
}
