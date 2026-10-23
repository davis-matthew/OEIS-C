/*
 * A171200  G.f. satisfies A(x) = 1 + x*A(2x)^3.
 * https://oeis.org/A171200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2^(n-1) * Sum_{i=0..n-1} Sum_{j=0..n-i-1} a(i)
 *            * a(j) * a(n-i-j-1) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoization buffers */
    static seq_t *cache  = NULL;          /* a[0..max_n]          */
    static seq_t *conv   = NULL;          /* conv[k] = Σ_{i+j=k} a[i]*a[j]  */
    static idx_t  max_n  = 0;             /* highest index computed  */
    static size_t cache_cap = 0;          /* allocated capacity for a   */
    static size_t conv_cap  = 0;          /* allocated capacity for conv*/
    static seq_t two_pow = 0;             /* 2^(m-1) for current m  */

    /* initialise on first call */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
        conv_cap = 1;
        conv = (seq_t*)malloc(conv_cap * sizeof(seq_t));
        conv[0] = 1;          /* conv[0] = a0*a0 = 1 */
        max_n = 0;
        two_pow = 1;          /* 2^0 = 1 */
    }

    /* if the requested n is already computed, return it */
    if (n <= max_n)
        return cache[n];

    /* expand the memoisation up to n */
    for (idx_t m = max_n + 1; m <= n; ++m)
    {
        /* compute the triple convolution at position m-1
           using the stored pair convolution conv[]   */
        seq_t sum = 0;
        idx_t mid = m - 1;
        for (idx_t k = 0; k <= mid; ++k)
            sum += cache[k] * conv[mid - k];

        /* compute a(m) = 2^(m-1) * sum  */
        seq_t val = sum * two_pow;   /* two_pow holds 2^(m-1) for this m */

        /* ensure cache is large enough */
        if ((size_t)m >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= (size_t)m) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[m] = val;

        /* update two_pow for the next iteration (next m): 2^m */
        two_pow = two_pow * 2;   /* operator* works for intrinsic types
                                    and for arbitrary-precision classes   */

        /* ensure conv has space up to index 2*m */
        size_t need_cap = 2 * (size_t)m + 1;
        if (need_cap > conv_cap)
        {
            size_t new_cap = conv_cap ? conv_cap * 2 : 1;
            while (new_cap < need_cap) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(conv, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            memset(tmp + conv_cap, 0, (new_cap - conv_cap) * sizeof(seq_t));
            conv = tmp;
            conv_cap = new_cap;
        }

        /* update the pair convolution conv[] with pairs involving a[m]   */
        for (idx_t i = 0; i <= m; ++i)
        {
            seq_t prod = cache[i] * cache[m];
            conv[i + m] += prod;   /* pair (i,m)   */
            if (i != m)           
                conv[i + m] += prod;   /* pair (m,i)   */
        }
    }

    max_n = n;
    return cache[n];
}
