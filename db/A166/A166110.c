/*
 * A166110  A positive integer n is included if the longest contiguous run of
 *          similar digits in binary n is of 0's.
 * https://oeis.org/A166110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   has_longest_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166110:
   a(n) = the n-th positive integer whose binary representation contains
   a longest contiguous run of zeros (ties with ones allowed). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_longest_zero.h"

static seq_t *cache = NULL;        /* cached a(k) values */
static idx_t cache_size = 0;       /* capacity of cache array   */
static idx_t count = 0;            /* number of values stored   */
static idx_t next_test = 1;        /* next integer to test     */

/* Expand cache array to accommodate at least `needed` elements. */
static void expand_cache(idx_t needed)
{
    if (cache_size >= needed) return;
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size < needed) new_size <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_ptr) {
        /* In a production setting we would handle the error; for simplicity we abort. */
        abort();
    }
    cache = new_ptr;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Return cached value if possible. */
    if (n <= count) return cache[n - 1];

    /* Extend cache until we have at least n values. */
    while (count < n) {
        if (has_longest_zero(next_test)) {
            expand_cache(count + 1);
            cache[count] = (seq_t)next_test;
            count++;
        }
        next_test++;
    }
    return cache[n - 1];
}
