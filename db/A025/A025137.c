/*
 * A025137  a(n) = 3rd elementary symmetric function of C(n,0), C(n,1), ...,
 *          C(n,[ n/2 ]).
 * https://oeis.org/A025137
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3rd elementary symmetric function of binomial
 *            coefficients C(n,0), C(n,1), …, C(n,⌊n/2⌋)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n / 2;

    seq_t cnk = 1;      /* C(n,0) */
    seq_t sumS1 = 0;    /* sum of C(n,i) for i < current k */
    seq_t pairSum = 0;  /* sum of C(n,i)*C(n,j) for i<j<current k */
    seq_t e3 = 0;       /* 3rd elementary symmetric sum */

    for (idx_t k = 0; k <= m; ++k)
    {
        /* add contribution of current element to e3 */
        e3 += cnk * pairSum;

        /* update pairSum with pairs involving current element */
        pairSum += cnk * sumS1;

        /* include current element in sumS1 for later steps */
        sumS1 += cnk;

        /* compute next binomial coefficient, if any */
        if (k < m)
        {
            cnk = cnk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    return e3;
}
