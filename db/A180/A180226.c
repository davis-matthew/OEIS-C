/*
 * A180226  a(n) = 4*a(n-1) + 10*a(n-2), with a(1)=0 and a(2)=1.
 * https://oeis.org/A180226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2), a(1)=0, a(2)=1 (OEIS A180226)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* Cache holds a[0 .. cached_limit-1] corresponding to a(1)..a(cached_limit) */
    static seq_t *cache = NULL;
    static idx_t cached_limit = 0;

    if (n == 0)                /* below offset – behaviour irrelevant */
        return (seq_t)0;

    /* Return cached value if already computed */
    if (n <= cached_limit)
        return cache[n - 1];

    /* Allocate new cache of size n (at least up to the requested index) */
    seq_t *new_cache = seq_alloc_new_array((size_t)n);
    if (!new_cache)            /* allocation failure – fallback to recompute */
    {
        new_cache = cache;
        cached_limit = 0;
        if (n <= cached_limit)
            return new_cache[n - 1];
    }

    /* Copy existing cached entries */
    if (cache && cached_limit > 0)
    {
        for (idx_t i = 0; i < cached_limit; ++i)
            new_cache[i] = cache[i];
    }

    /* Initialize the first terms if they were missing in the previous cache */
    if (cached_limit < 1)
        new_cache[0] = (seq_t)0;           /* a(1) */
    if (cached_limit < 2 && n >= 2)
        new_cache[1] = (seq_t)1;           /* a(2) */

    /* Determine starting index for iterative computation */
    idx_t start = cached_limit + 1;
    if (start < 3) start = 3;              /* need two previous terms */

    /* Compute remaining terms iteratively using only integer arithmetics */
    for (idx_t i = start; i <= n; ++i)
    {
        /* 4 * a(i-1) */
        seq_t t4 = new_cache[i - 2];       /* a(i-1) is at index i-2 (a(1) -> idx 0) */
        t4 += t4;                          /* 2 * a(i-1) */
        t4 += t4;                          /* 4 * a(i-1) */

        /* 10 * a(i-2) */
        seq_t t10 = new_cache[i - 3];      /* a(i-2) is at index i-3 */
        for (int k = 1; k < 10; ++k)
            t10 += new_cache[i - 3];

        new_cache[i - 1] = t4 + t10;        /* a(i) is stored at index i-1 */
    }

    /* Replace old cache, preserving destructor semantics */
    if (cache)
        seq_free(cache);

    cache = new_cache;
    cached_limit = n;

    return cache[n - 1];
}
