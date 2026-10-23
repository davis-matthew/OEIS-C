/*
 * A175016  y-values in the solution to x^2-58*y^2=1.
 * https://oeis.org/A175016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39206*a(n-1) - a(n-2), a(1)=0, a(2)=2574
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 none,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"   /* free_seq_array(seq_t*) */

seq_t a(idx_t n)
{
    /* Cache of already computed terms; cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of filled terms */

    if (n < 1)   /* unspecified behaviour for n==0 */
        return (seq_t)0;

    /* Expand cache if needed */
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *newcache = alloc_seq_array(newcap);
        for (size_t i = 0; i < len; ++i)
            newcache[i] = cache[i];
        if (cache) free_seq_array(cache);
        cache = newcache;
        cap   = newcap;
    }

    /* Initialise the first two values if not yet done */
    if (len < 2) {
        cache[0] = (seq_t)0;      /* a(1) */
        cache[1] = (seq_t)2574;   /* a(2) */
        len = 2;
    }

    /* Compute missing terms up to n */
    while (len < (size_t)n) {
        seq_t val = ((seq_t)39206) * cache[len - 1] - cache[len - 2];
        cache[len] = val;
        ++len;
    }

    return cache[n - 1];
}
