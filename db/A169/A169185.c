/*
 * A169185  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169185:
 *   G.f.  = (t^27 + 2t^26 + … + 2t + 1) /
 *           (351t^27 – 26t^26 – … – 26t + 1)
 * Recurrence:
 *   a(0) = 1
 *   for n ≥ 1
 *     a(n) = 26·∑_{k=1}^{26} a(n–k)  – 351·a(n–27) + c_n
 *     c_n = 2  for 1 ≤ n ≤ 26
 *           1  for n  = 27
 *           0  for n ≥ 28
 */
 
#include <stdlib.h>
#include <stddef.h>
 
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* buffer of already computed terms   */
    static size_t cache_cap = 0;    /* number of allocated elements       */
    static idx_t  last_computed = 0;/* index of the largest computed term */
 
    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate at least one slot for a(0) */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1; /* a(0) = 1 */
        last_computed = 0;
    }
 
    /* expand cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {           /* allocation failed – abort */
            exit(1);
        }
        /* initialise new elements to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
 
    /* compute terms that are not yet in the cache */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
 
        /* 26·∑_{k=1}^{26} a(i–k)  (ignore terms with i<k) */
        for (idx_t k = 1; k <= 26 && i >= k; ++k)
            sum += (seq_t)26 * cache[(size_t)(i - k)];
 
        /* –351·a(i–27)  (if i≥27) */
        if (i >= 27)
            sum -= (seq_t)351 * cache[(size_t)(i - 27)];
 
        /* constant term c_i */
        seq_t add = (seq_t)0;
        if (i >= 1 && i <= 26)
            add = (seq_t)2;
        else if (i == 27)
            add = (seq_t)1;
        /* else add remains 0 for i≥28 */
 
        cache[(size_t)i] = sum + add;
    }
 
    last_computed = n;
    return cache[(size_t)n];
}
