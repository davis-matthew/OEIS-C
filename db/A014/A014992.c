/*
 * A014992  a(n) = (1 - (-10)^n)/11.
 * https://oeis.org/A014992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-10)^n)/11
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 39,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence a(n) = -9*a(n-1) + 10*a(n-2) with
   a(1) = 1, a(2) = -9.  The sequence is memoised. */
#include "../utils/alloc_seq_array.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  Capacity is fixed to
       accommodate all n the problem promises (≈10000). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms computed so far */
    const idx_t CAPACITY = 10000 + 1;    /* indices 0..10000 are usable */

    if (!cache) {
        cache = alloc_seq_array(CAPACITY);   /* allocates CAPACITY elements */
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;           /* a(1) */
        } else if (i == 2) {
            cache[1] = (seq_t)-9;          /* a(2) */
        } else {
            /* a(i) = -9*a(i-1) + 10*a(i-2) */
            cache[i-1] = ( (seq_t)(-9) * cache[i-2] )
                        + ( (seq_t)10 * cache[i-3] );
        }
    }
    cache_len = n;
    return cache[n-1];                     /* a(n) */
}
