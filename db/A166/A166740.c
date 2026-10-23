/*
 * A166740  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A166740: a(n) computed from the linear recurrence
   a(n) = 45*a(n-1)+45*a(n-2)+…+45*a(n-11) – 1035*a(n-12) + c(n),
   where c(0)=1, c(1)…c(11)=2, c(12)=1, and c(n)=0 for n>12.
   This matches the given generating function.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_cap  = 0;      /* allocated capacity (elements)    */
    static size_t cache_len  = 0;      /* number of terms already computed */

    /* ensure we have room for n+1 elements */
    size_t nn = (size_t)n;
    if (nn + 1 > cache_cap) {
        size_t new_cap = nn + 1;
        /* realloc with cast to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise cache if first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;           /* a(0) */
        cache_len = 1;
    }

    /* compute missing values up to n */
    if (nn + 1 > cache_len) {
        for (size_t i = cache_len; i <= nn; ++i) {
            seq_t sum = 0;

            /* 45 * a(i-1)..a(i-11) */
            size_t kmax = (i < 11) ? i : 11;
            for (size_t k = 1; k <= kmax; ++k) {
                sum += (seq_t)45 * cache[i - k];
            }

            /* –1035 * a(i-12) if i >= 12 */
            if (i >= 12) {
                sum -= (seq_t)1035 * cache[i - 12];
            }

            /* add coefficient c(i) from numerator */
            seq_t coeff;
            if (i == 0)            coeff = 1;
            else if (i <= 11)      coeff = 2;
            else if (i == 12)      coeff = 1;
            else                   coeff = 0;

            sum += coeff;
            cache[i] = sum;
        }
        cache_len = nn + 1;
    }

    return cache[nn];
}
