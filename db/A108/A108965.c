/*
 * A108965  Numbers n with property that for each digit in n there is another
 *          digit in n differing from it by 1.
 * https://oeis.org/A108965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth positive integer all of whose decimal digits each
 *            have another digit in the integer differing by 1
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   has_adjacent_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iteratively tests integers in increasing order and keeps a
   cache of all numbers satisfying the property.  On each call it extends the
   cache up to the requested index.  The test for a candidate x is
   performed by counting the distinct digits present in x; for every digit d
   present, at least one of d-1 or d+1 must also be present. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_digits.h"

/* Cache of computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of terms stored (indexed from 1) */
static idx_t cache_cap  = 0;   /* capacity of cache array */
static idx_t next_candidate = 10;  /* next integer to test for the property */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined for offset 1; return 0 */
        return (seq_t)0;

    /* Grow the cache until it contains term n. */
    while (cache_size < n) {
        if (has_adjacent_digits(next_candidate)) {
            /* Expand cache if needed. */
            if (cache_size + 1 > cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[++cache_size] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n];
}
