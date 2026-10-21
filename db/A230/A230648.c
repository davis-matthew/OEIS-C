/*
 * A230648  Number of nX4 0..2 white square subarrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) - a(n-3) - a(n-4) (n > 5) Initial
 *            values: a(1)=0, a(2)=9, a(3)=21, a(4)=123, a(5)=531
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑indexed cache of computed terms */
static idx_t  cache_cap   = 0;   /* current allocated size of cache */
static idx_t  max_computed = 0;  /* largest index that has been computed */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* safety: sequence is defined from 1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 6;  /* indices 0..5 are needed for initial values */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */

        cache[0] = (seq_t)0;             /* unused, but keep defined */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)21;
        cache[4] = (seq_t)123;
        cache[5] = (seq_t)531;
        max_computed = 5;
    }

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    while (max_computed < n) {
        idx_t   i = max_computed + 1;
        seq_t   v = (seq_t)3 * cache[i-1]
                 + (seq_t)8 * cache[i-2]
                 -             cache[i-3]
                 -             cache[i-4];
        cache[i] = v;
        max_computed = i;
    }

    return cache[n];
}
