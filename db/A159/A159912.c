/*
 * A159912  Partial sums of A159913(k) = 2^bitcount(2k+1)-1 = A038573(2k+1),
 *          bitcount=A000120.
 * https://oeis.org/A159912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{n-1} (2^popcount(2*i+1) - 1) * =
 *            2*sum_{i=0}^{n-1} 2^popcount(i) - n
 * Range:     u8 n <= 25, u16 n <= 786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/popcount_u64.h"

/* popcount of a 64‑bit integer.  Use the compiler builtin. */

seq_t a(idx_t n)
{
    /* Store computed values in a static cache so that calls in any order
     * are fast.  The cache holds a(k) for k = 0 … (len-1).  */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* allocated cells in cache */
    static size_t len   = 0;        /* number of values currently stored */

    /* Special case: a(0) = 0.  */
    if (n == 0)
    {
        /* Ensure that cache[0] exists for future requests. */
        if (cache == NULL)
        {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) return 0;          /* allocation failure */
            cache[0] = 0;
            cap = 1;
            len = 1;
        }
        return cache[0];
    }

    /* Allocate or grow the cache to hold at least (n+1) elements. */
    if (cache == NULL)
    {
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 0;        /* a(0) = 0 */
        len = 1;
    }
    else if ((size_t)n + 1 > cap)
    {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap)
            newcap *= 2;          /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to a(n). */
    for (idx_t i = len - 1; i < n; ++i)
    {
        /* 2*i + 1 fits in 64 bits for the sizes of interest. */
        uint64_t t = (i << 1) + 1;
        int pc = popcount_u64(t);
        seq_t val = ((seq_t)1 << (idx_t)pc) - 1;  /* 2^popcount - 1 */
        cache[i + 1] = cache[i] + val;
    }
    len = (size_t)n + 1;

    return cache[n];
}
