/*
 * A043424  Numbers having four 0's in base 8.
 * https://oeis.org/A043424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   has_four_zeroes_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers having four 0's in base 8
   a(n) = the n-th positive integer whose octal representation
           contains exactly four 0 digits. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_four_zeroes_oct.h"

/* Cached terms as idx_t (they fit in 64 bits for all n we seek),
   stored this way to avoid construction issues with arbitrary‑precision
   types.  The cache is lazily grown as needed. */
static idx_t *cache       = NULL;   /* array of idx_t values              */
static size_t cache_size  = 0;       /* number of terms stored              */
static size_t cache_cap   = 0;       /* current capacity of the array        */
static idx_t next_val     = 1;       /* smallest integer not yet tested     */

/* Grow the cache until it can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    /* In the unlikely event of allocation failure, abort the program */
    if (!tmp)
        abort();

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined position – harmless return */
        return 0;

    /* Repeatedly test integers until we have found at least n terms. */
    while (cache_size < n) {
        if (has_four_zeroes_oct(next_val)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = next_val;
        }
        ++next_val;
    }

    /* Cast the cached idx_t value to the output type seq_t. */
    return (seq_t)cache[n - 1];
}
