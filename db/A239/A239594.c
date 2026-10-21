/*
 * A239594  Number of n X 2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its northwest
 *          or one plus the sum of the elements antidiagonally to its
 *          northeast, modulo 4.
 * https://oeis.org/A239594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 101, u64 n <= 206,
 *            u128 n <= 210, big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239594.
   Empirical recurrence (valid for n > 6):
       a(n) = a(n-2) + a(n-5)
   Initial values:
       a(1)=4, a(2)=3, a(3)=4, a(4)=4, a(5)=5, a(6)=7, a(7)=8
   The function is memoised; it works for any order of calls
   and runs in O(n) time for computing up to the maximum requested n. */

#include <stdlib.h>                 /* for abort() */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the sequence offset (not required). */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t sz = 0;          /* number of cached entries (cache[0] unused) */

    /* Grow the cache to accommodate index n (inclusive). */
    if ((size_t)n >= sz) {
        size_t new_sz = (size_t)n + 1;   /* allocate up to and including n */
        seq_t *tmp = seq_realloc(cache, sz, new_sz);
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        /* Fill newly allocated entries. */
        for (size_t i = sz; i < new_sz; ++i) {
            if (i == 1)          cache[i] = (seq_t)4;
            else if (i == 2)     cache[i] = (seq_t)3;
            else if (i == 3)     cache[i] = (seq_t)4;
            else if (i == 4)     cache[i] = (seq_t)4;
            else if (i == 5)     cache[i] = (seq_t)5;
            else if (i == 6)     cache[i] = (seq_t)7;
            else if (i == 7)     cache[i] = (seq_t)8;
            else                cache[i] = cache[i-2] + cache[i-5];
        }
        sz = new_sz;
    }

    return cache[n];
}
