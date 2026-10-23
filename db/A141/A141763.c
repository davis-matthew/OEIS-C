/*
 * A141763  Column 3 of triangle A141760.
 * https://oeis.org/A141763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - \sum_{j=0}^{n-1} a(j) * (-1)^{n-j} * * C(
 *            ((j+2)(j+3)/2)+n-j-4 , n-j ) , a(0)=1 This uses the recurrence
 *            given in the OEIS entry. The function memoises previously
 *            computed values so that any call order is supported with O(n^2)
 *            overall time for the first n terms
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;   /* allocated number of elements      */
    static size_t len    = 0;   /* highest index already computed */

    /* Allocate and initialise cache if this is the first call */
    if (cache == NULL) {
        cap = (n >= 1) ? (n + 1) : 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[0] = 1;
        len = 0;
    }

    /* Grow cache if we need more space */
    if (n + 1 > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
        cap = n + 1;
    }

    /* Compute any missing terms up to n */
    while (len < n) {
        idx_t idx = len + 1;        /* index of the new term */
        seq_t sum = 0;

        for (idx_t j = 0; j < idx; ++j) {
            idx_t b   = idx - j;                                       /* n-j  */
            idx_t t   = ((j + 2) * (j + 3)) / 2;                       /* ((j+2)(j+3))/2 */
            idx_t a_val = t + b - 4;                                   /* upper index */

            seq_t bin = 0;
            if (a_val >= b)                     /* binomial defined only if a>=b */
                bin = binom(a_val, b);

            if (bin == 0) continue;

            seq_t prod = cache[j] * bin;
            if ((b & 1) == 0) {                 /* even exponent -> positive sign */
                sum += prod;
            } else {                            /* odd exponent -> negative sign */
                sum -= prod;
            }
        }

        /* a(idx) = 1 - sum */
        seq_t an = 1;
        an -= sum;
        cache[idx] = an;
        len = idx;
    }

    return cache[n];
}
