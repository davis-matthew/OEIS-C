/*
 * A230676  Number of 2 X n 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A230676: a(n) = 5*a(n-1) + 6*a(n-2) + 20*a(n-3) + 12*a(n-4) + 8*a(n-5),
   with a(1)=0, a(2)=15, a(3)=87, a(4)=513, a(5)=3387. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static size_t cache_cap = 0;     /* allocated size of cache (>= n+1) */
static size_t computed = 0;      /* largest n for which cache[n] is valid */

/* Ensure cache can hold indices up to n (inclusive) */
static void _ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;      /* one slot for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        /* initialise newly allocated slots to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined region, return 0 */

    _ensure_cache(n);

    /* Compute missing terms up to n */
    if (computed < (size_t)n) {
        for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)0;
            else if (i == 2) cache[i] = (seq_t)15;
            else if (i == 3) cache[i] = (seq_t)87;
            else if (i == 4) cache[i] = (seq_t)513;
            else if (i == 5) cache[i] = (seq_t)3387;
            else
                cache[i] = (seq_t)5  * cache[i-1]
                         + (seq_t)6  * cache[i-2]
                         + (seq_t)20 * cache[i-3]
                         + (seq_t)12 * cache[i-4]
                         + (seq_t)8  * cache[i-5];
        }
        computed = (size_t)n;
    }

    return cache[n];
}
