/*
 * A012261  Expansion of e.g.f. exp(arctanh(sinh(x))).
 * https://oeis.org/A012261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(0)=1 and for n>0 a(n) = ( R[n] -
 *            sum_{k=1}^{n-1} a[k] * a[n-k] * C(n,k) ) / 2 where R[0]=1 and
 *            for n>0 R[n] = [n odd] + sum_{odd k=1}^n C(n,k) * R[n-k] (C(n,k)
 *            binomial)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized tables */
    static seq_t *A = NULL;     /* a( ) values  */
    static seq_t *R = NULL;     /* R( ) values  */
    static idx_t   size = 0;    /* current allocated size  */

    /* grow tables if necessary */
    if (n >= size)
    {
        idx_t newsize = (size == 0 ? 16 : size * 2);
        while (newsize <= n) newsize *= 2;

        seq_t *newA = (seq_t*)realloc(A, newsize * sizeof(seq_t));
        seq_t *newR = (seq_t*)realloc(R, newsize * sizeof(seq_t));
        if (!newA || !newR) return 0;   /* allocation failed */
        A = newA;
        R = newR;

        /* initialise new entries */
        for (idx_t i = size; i < newsize; ++i)
        {
            A[i] = (seq_t)0;
            R[i] = (seq_t)0;
        }
        /* base values */
        if (size == 0)
        {
            A[0] = (seq_t)1;
            R[0] = (seq_t)1;
            size = 1;
        }
        /* compute values up to newsize-1 */
        for (idx_t m = size; m < newsize; ++m)
        {
            /* ---- compute R[m] ---- */
            seq_t r = (seq_t)0;
            if (m & 1) r += (seq_t)1;            /* n odd term */
            /* binomial coefficient iteratively */
            seq_t comb = (seq_t)1;
            for (idx_t k = 1; k <= m; ++k)
            {
                comb = comb * (seq_t)(m - k + 1);
                comb = comb / (seq_t)k;
                if (k & 1)                       /* only odd k */
                    r += comb * R[m - k];
            }
            R[m] = r;

            /* ---- compute A[m] ---- */
            seq_t sum = (seq_t)0;
            comb = (seq_t)1;
            for (idx_t k = 1; k <= m - 1; ++k)
            {
                comb = comb * (seq_t)(m - k + 1);
                comb = comb / (seq_t)k;
                sum += comb * A[k] * A[m - k];
            }
            A[m] = (R[m] - sum) / (seq_t)2;
        }
        size = newsize;
    }
    return A[n];
}
