/*
 * A125094  Eigensequence of A061554 regarded as a triangle: a(n) =
 *          Sum_{k=0..n-1} A061554(n-1,k)*a(k) with a(0)=1.
 * https://oeis.org/A125094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} a(k) * binom(n-1, floor((n -
 *            (-1)^(n-k-1)*(k+1))/2)) with a(0)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t  last  = 0;   /* largest index already computed */

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cap  = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* compute missing terms up to n (if any) */
    if (n > last) {
        /* ensure enough space */
        if ((size_t)(n + 1) > cap) {
            cap = (n + 1) * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0; /* allocation failed */
        }

        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t sum = (seq_t)0;

            /* compute a(m) from the recurrence */
            for (idx_t k = 0; k < m; ++k) {
                /* diff = m-1-k, used to determine the sign */
                idx_t diff = m - 1 - k;          /* diff >= 0 for all k < m */
                int   sign = (diff & 1) ? -1 : 1;/* (-1)^{diff} */

                idx_t t;
                if (sign == 1)
                    t = m - (k + 1);   /* n - (k+1)          */
                else
                    t = m + (k + 1);   /* n + (k+1)          */

                idx_t r = t >> 1;    /* floor(t/2) */

                /* binomial coefficient */
                seq_t bin = binom(m - 1, r);

                sum += cache[k] * bin;
            }

            cache[m] = sum;
        }

        last = n;
    }

    return cache[n];
}
