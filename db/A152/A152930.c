/*
 * A152930  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          k 4-gonal polygonal components chained with string components of
 *          length 2 as k varies.
 * https://oeis.org/A152930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) - a(n-2); a(1)=7, a(2)=176
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of terms, 0‑based for a(1) */
    static idx_t cache_len = 0;      /* number of terms stored (a(1) .. a(cache_len)) */
    static size_t cache_cap = 0;     /* allocated capacity of cache */

    /* n is expected to be >= 1; behaviour for n==0 is undefined */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)7;                  /* a(1) */
        cache[1] = (seq_t)176;                /* a(2) */
        cache_len = 2;
    }

    /* return already computed value */
    if (n <= cache_len)
        return cache[n - 1];

    /* ensure cache can hold up to term n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap = new_cap ? new_cap * 2 : 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* zero‑based indices for array storage */
        idx_t idx  = i - 1;                  /* position for a(i) */
        seq_t prev1 = cache[idx - 1];         /* a(i-1) */
        seq_t prev2 = cache[idx - 2];         /* a(i-2) */
        seq_t val = (seq_t)25 * prev1 - prev2;
        cache[idx] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
