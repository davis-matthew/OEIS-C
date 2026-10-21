/*
 * A230647  Number of nX3 0..2 white square subarrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230647
   Formula: a(n) = 9*a(n-2) - 4*a(n-4),      a(1)=0, a(2)=3, a(3)=3, a(4)=21
   (recurrence valid for all n ≥ 5)
*/

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static idx_t max_computed = 0;

/* Return the n-th term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    /* Out-of-range requests return 0 */
    if (n < 1)
        return (seq_t)0;

    /* Initialise cache with the first four terms */
    if (!cache)
    {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */

        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)3;   /* a(2) */
        cache[3] = (seq_t)3;   /* a(3) */
        cache[4] = (seq_t)21;  /* a(4) */

        max_computed = 4;
    }

    /* Return already computed value */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache to hold up to index n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                  /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
            val = (seq_t)0;
        else if (i == 2)
            val = (seq_t)3;
        else if (i == 3)
            val = (seq_t)3;
        else if (i == 4)
            val = (seq_t)21;
        else
            val = (seq_t)9 * cache[i - 2] - (seq_t)4 * cache[i - 4];

        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
