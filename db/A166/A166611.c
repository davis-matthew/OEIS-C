/*
 * A166611  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n + 22 * Σ_{k=1..min(11,n)} a(n-k) - 253 * a(n-12) with
 *            p_0 = 1, p_1 … p_11 = 2, p_12 = 1, and p_n = 0 for n > 12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed values      */
    static size_t cache_cap = 0;    /* allocated capacity           */
    static idx_t  cache_len = 0;    /* number of values stored      */

    /* constants p_0..p_12 */
    static const seq_t p[13] = {1,2,2,2,2,2,2,2,2,2,2,2,1};

    /* initialise cache if first call */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        cache_len = 1;
    }

    /* return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* grow cache to store up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (i <= 12) ? p[i] : (seq_t)0;

        /* add 22 * a(i-k) for k = 1 .. min(11,i) */
        idx_t klimit = (i < 12) ? i : (idx_t)11;
        for (idx_t k = 1; k <= klimit; ++k)
            sum += (seq_t)22 * cache[i - k];

        /* subtract 253 * a(i-12) when i >= 12 */
        if (i >= 12)
            sum -= (seq_t)253 * cache[i - 12];

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
