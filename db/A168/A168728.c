/*
 * A168728  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=3; a(n)=2*a(n-1) for 2≤n≤17;
 *            a(18)=∑_{i=1}^{17}a(i)-a(0)+1; a(n)=∑_{i=1}^{17}a(n-i)-a(n-18)
 *            for n>18. (This recurrence comes from the given generating
 *            function.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t   max_n  = (idx_t)-1;
    static seq_t    sum18 = 0;   /* sum of the last 18 terms, valid when max_n>=18 */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely */
        cache[0] = (seq_t)1;
        max_n   = 0;
    }

    /* if requested value already computed, return it */
    if (n <= max_n) return cache[n];

    /* compute successive terms until reaching n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* grow the array if necessary */
        if (cap <= i) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        if (i <= 17) {            /* initial exponential part   */
            if (i == 1)
                cache[1] = (seq_t)3;
            else
                cache[i] = cache[i - 1] + cache[i - 1];
        }
        else if (i == 18) {       /* first special value       */
            seq_t s = 0;
            for (idx_t j = 0; j < 18; ++j) s += cache[j];   /* sum a[0..17] */
            cache[18] = s - (seq_t)1;   /* a(18) = sum_{1..17} - a(0) + 1 = sum_{0..17} - 1 */
            /* new sum18 = sum of a[1..18] */
            sum18 = s - cache[0] + cache[18];
        }
        else {                    /* recurrence for n ≥ 19       */
            seq_t sub = cache[i - 18];
            sub += sub;               /* 2 * a(i-18) */
            cache[i] = sum18 - sub;
            sum18 = sum18 - cache[i - 18] + cache[i];
        }
    }
    max_n = n;
    return cache[n];
}
