/*
 * A043939  Numbers k such that 1 and 2 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_12_21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th integer that contains the adjacent digits 1
   and 2 (in either order) in its octal representation, but k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_12_21.h"

/* Cache of found terms                                    */
static seq_t *cache = NULL;   /* array of a(1), a(2), …              */
static size_t cache_size  = 0;
static size_t cache_cap   = 0;
static seq_t last_examined = 0;   /* largest k already checked          */

/* Ensure cache contains at least n terms                           */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    /* Allocate or grow the cache array if needed                   */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Scan consecutive integers, adding those that satisfy the
       definition until we have enough terms.                     */
    seq_t k = last_examined + (seq_t)1;
    while (cache_size < (size_t)n) {
        if (has_adjacent_12_21(k) && !has_adjacent_12_21(k + (seq_t)1))
            cache[cache_size++] = k;
        k++;
    }
    last_examined = k - (seq_t)1;
}

/* External linkage as required                                    */
seq_t a(idx_t n)
{
    /* n is 1‑based; return the nth term of the sequence           */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
