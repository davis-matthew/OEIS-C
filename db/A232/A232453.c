/*
 * A232453  Number of n X 2 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, vertically, diagonally or
 *          antidiagonally.
 * https://oeis.org/A232453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 12*a(n-2) + 39*a(n-3) for n≥4, * with a(1)=3,
 *            a(2)=51, a(3)=381
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache the sequence values to handle arbitrary call orders. */
    static seq_t *cache = NULL;   /* array of computed terms, zero‑based    */
    static size_t cache_cap = 0;  /* allocated size of cache array          */
    static size_t cache_len = 0;  /* number of terms actually computed      */

    if (n == 0)          /* offset is 1; behaviour below is irrelevant */
        return 0;

    /* Ensure cache has enough capacity for n terms. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – give up silently (value will be wrong) */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already done. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)3;
        else if (i == 2)
            val = (seq_t)51;
        else if (i == 3)
            val = (seq_t)381;
        else
            /* a(i)=6*a(i-1)+12*a(i-2)+39*a(i-3) */
            val = (seq_t)6  * cache[(size_t)(i-2)] +
                  (seq_t)12 * cache[(size_t)(i-3)] +
                  (seq_t)39 * cache[(size_t)(i-4)];

        cache[(size_t)(i-1)] = val;   /* store as zero‑based: a(i) at cache[i-1] */
    }
    cache_len = (size_t)n;  /* mark terms up to n as computed */

    return cache[(size_t)(n-1)];
}
