/*
 * A229505  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          diagonally and antidiagonally with exactly one mistake, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 36*a(n-2) for n > 3, a(1) = 0, a(2) = 48,
 *            a(3) = 480 (empirical recurrence)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *a_cache = NULL;   /* array of cached values, 1-based indexing */
static idx_t a_cache_max = 0;   /* largest n for which a_cache[n] is valid */
static size_t a_cache_alloc = 0;/* allocated size of a_cache (indices 0..size-1) */

/* Returns the n-th term of OEIS A229505 (n >= 1).  The function is
   memoized: subsequent calls are O(1) after the first computation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, although not used */

    /* allocate initial cache if needed */
    if (a_cache == NULL) {
        size_t init_size = (n > 3 ? n + 1 : 4);      /* indices 0..init_size-1 */
        a_cache = alloc_seq_array(init_size);
        if (a_cache == NULL) return (seq_t)0;        /* allocation failed */
        a_cache_alloc = init_size;
        /* initialize known initial terms */
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)48;
        a_cache[3] = (seq_t)480;
        a_cache_max = 3;
    }

    /* grow cache if required */
    if (n + 1 > a_cache_alloc) {
        size_t new_size = n + 1;
        seq_t *new_cache = alloc_seq_array(new_size);
        if (new_cache == NULL) return (seq_t)0;        /* allocation failed */
        /* copy existing values */
        for (idx_t i = 1; i <= a_cache_max; ++i)
            new_cache[i] = a_cache[i];
        free_seq_array(a_cache);
        a_cache = new_cache;
        a_cache_alloc = new_size;
    }

    /* compute missing terms up to n */
    if (n > a_cache_max) {
        for (idx_t i = a_cache_max + 1; i <= n; ++i) {
            /* recurrence valid for i >= 4 */
            a_cache[i] = ((seq_t)12) * a_cache[i - 1]
                       - ((seq_t)36) * a_cache[i - 2];
        }
        a_cache_max = n;
    }

    return a_cache[n];
}
