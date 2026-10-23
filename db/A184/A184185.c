/*
 * A184185  Number of permutations of {1,2,...,n} having no cycles of the form
 *          (i, i+1, i+2, ..., i+j-1) (j >= 1).
 * https://oeis.org/A184185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n-1) - 2*(n-1)*a(n-2) + (n-2)*a(n-3) (n>2)
 *            Initial values: a(0)=1, a(1)=0, a(2)=0 Recurrence holds for all
 *            n≥3
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc */

seq_t a(idx_t n)
{
    /* --- memoization data ----------------------------------- */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static idx_t  cache_len = 0;    /* number of elements already computed (next free index) */
    static size_t cache_cap = 0;    /* allocated capacity of cache array */

    /* --- if value already known, return it ------------------- */
    if (n < cache_len)
        return cache[n];

    /* --- enlarge cache if necessary -------------------------- */
    if (n + 1 > cache_cap)
    {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failed: return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
    }

    /* --- compute values from the first missing one up to n ---- */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        if (k == 0)
            cache[0] = (seq_t)1;
        else if (k == 1)
            cache[1] = (seq_t)0;
        else if (k == 2)
            cache[2] = (seq_t)0;
        else
        {
            /* recurrence relation */
            seq_t term1 = (seq_t)(k + 2) * cache[k - 1];
            seq_t term2 = (seq_t)2 * (seq_t)(k - 1) * cache[k - 2];
            seq_t term3 = (seq_t)(k - 2) * cache[k - 3];
            cache[k] = term1 - term2 + term3;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
