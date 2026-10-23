/*
 * A168975  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168975 – number of reduced words of length n in the Coxeter
    group on 10 generators with (S_i)^2 = (S_iS_j)^3 = 1.
    Recurrence (order 23 derived from the rational generating function):

        a(0) = 1;

        for n ≥ 1
          let  coeff = (n ≤ 23 ? ((n==0||n==23) ? 1 : 2) : 0);
          a(n) = coeff
                 + 8 * Σ_{k=1}^{min(22,n)} a(n-k)
                 – (n==23 ? 36 * a(n-23) : 0);
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;     /* number of elements stored   */
    static size_t cache_cap = 0;     /* allocated capacity          */

    /* Ensure we have enough space for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to index n if not yet computed */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* coefficient from the numerator of the generating function  */
        seq_t val;
        if (i == 0 || i == 23)
            val = (seq_t)1;
        else if (i <= 23)
            val = (seq_t)2;
        else
            val = (seq_t)0;          /* i > 23   */

        /* add the terms coming from the denominator */
        /* d_k = -8 for 1 ≤ k ≤ 22,  d_23 = +36 */
        for (idx_t k = 1; k <= 23 && k <= i; ++k) {
            if (k == 23) {
                /* subtract 36 * a(i-23) */
                val -= (seq_t)36 * cache[i - 23];
            } else {
                /* add 8 * a(i-k) for k = 1 … 22   */
                val += (seq_t)8 * cache[i - k];
            }
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
