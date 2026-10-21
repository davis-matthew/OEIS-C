/*
 * A081407  4th-order non-linear ("factorial") recursion: a(0) = a(1) = a(2) =
 *          a(3) = 1, a(n) = (n+1)*a(n-4).
 * https://oeis.org/A081407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-4) for n>=4, with a(0)=a(1)=a(2)=a(3)=1;
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 31, u64 n <= 55, u128 n <= 95,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;          /* on-demand memoization buffer   */
static size_t  cache_cap = 0;        /* current buffer capacity (bytes)*/
static idx_t   computed  = 0;        /* last computed index (inclusive)*/

seq_t a(idx_t n)
{
    /* Return a cached value if already computed. */
    if (n < computed)
        return cache[n];

    /* Ensure the cache can hold indices 0..n. */
    grow_seq(&cache, &cache_cap, (size_t)(n + 1));

    /* Compute values from the next unused index up to n. */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i < 4)
            cache[i] = (seq_t)1;
        else
            cache[i] = (seq_t)(i + 1) * cache[i - 4];
    }

    computed = n + 1;      /* we have computed up to index n */
    return cache[n];
}
