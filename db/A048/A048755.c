/*
 * A048755  Partial sums of A048693.
 * https://oeis.org/A048755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 5; a(0)=1, a(1)=7
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This computes the partial sums of A048693 as defined in OEIS A048755. */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* handle the trivial case without allocating memory */
    if (n == 0) {
        return (seq_t)1;
    }

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity */
    static idx_t last  = 0;   /* highest index computed so far */

    /* initialise cache with first two values on first call */
    if (!cache) {
        /* allocate space for indices 0 and 1 */
        cache = seq_alloc_new_array(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cap   = 2;
        last  = 1;
    }

    /* if the requested value is already computed, return it */
    if (n <= last) {
        return cache[n];
    }

    /* Ensure the cache has enough capacity.  Allocate a new array and copy
       existing values if necessary.  (If using a grow helper, replace the
       allocation/copy logic with a call to grow_seq_array.) */
    if ((size_t)(n + 1) > cap) {
        seq_t *new_cache = seq_alloc_new_array(n + 1);
        for (idx_t i = 0; i <= last; ++i) {
            new_cache[i] = cache[i];
        }
        cache = new_cache;
        cap   = (size_t)(n + 1);
    }

    /* compute new terms by the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + cache[i - 2] + (seq_t)5;
    }
    last = n;

    return cache[n];
}
