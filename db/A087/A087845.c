/*
 * A087845  a[n] =a[Min[n-1,Abs[n - a[n-1]]]] + a[a[a[Abs[n - a[n-3]]]]].
 * https://oeis.org/A087845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a[n] = a[min(n-1, |n - a[n-1]|)] + a[a[a[|n - a[n-3]|]]] Initial
 *            values: a(1) = 1, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Base cases for n = 1, 2, 3 */
    if (n <= 3) {
        return (seq_t)1;
    }

    /* Static memoisation array */
    static seq_t *cache = NULL;   /* cache[0] is unused, indices start at 1 */
    static size_t cap   = 0;      /* capacity of cache   */
    static idx_t  max   = 3;      /* maximum n already computed */

    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        cap   = 4;                 /* indices 0 .. 3 are usable */
        cache = alloc_seq_array(cap);
        cache[0] = 0;              /* dummy to avoid using index 0 */
        cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Grow cache if we need to compute up to n */
    if (cap <= (size_t)n) {
        size_t newcap = cap;
        while (newcap <= (size_t)n)
            newcap <<= 1; /* double until big enough */

        seq_t *newcache = alloc_seq_array(newcap);
        /* copy existing values over */
        for (size_t i = 0; i < cap; ++i) {
            newcache[i] = cache[i];
        }
        free_seq_array(cache);
        cache = newcache;
        cap   = newcap;
    }

    /* Compute all terms up to n that have not yet been computed */
    for (idx_t i = max + 1; i <= n; ++i) {

        /* term1 = a[min(i-1, |i - a[i-1]|)] */
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t diff = (seq_t)i - prev;   /* i ≥ 1, prev ≤ i‑1 */
        idx_t idx1 = (diff < (seq_t)(i - 1)) ? (idx_t)diff : (i - 1);
        seq_t term1 = cache[(size_t)idx1];

        /* term2 = a[a[a[|i - a[i-3]|]]] */
        seq_t prev3 = cache[(size_t)(i - 3)];
        seq_t diff2 = (seq_t)i - prev3;
        idx_t idx2 = (idx_t)diff2;               /* |i - a[i-3]| is non‑negative */
        seq_t inner1 = cache[(size_t)idx2];
        seq_t inner2 = cache[(size_t)(idx_t)inner1];
        seq_t inner3 = cache[(size_t)(idx_t)inner2];

        cache[(size_t)i] = term1 + inner3;
    }

    max = n;
    return cache[(size_t)n];
}
