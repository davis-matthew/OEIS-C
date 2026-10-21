/*
 * A065755  Triangle of Gandhi polynomial coefficients.
 * https://oeis.org/A065755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065755 – Triangle of Gandhi polynomial coefficients
   Formula:  B(X,1) = X^5
              B(X,n) = X^5 · ( B(X+1,n-1) – B(X,n-1) )
   The (i,j)–th entry is the coefficient of X^(4+j) in B(X,i)
   The sequence a(n) is the triangle read row by row */
#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

#define MAX_DEG 400   /* enough for n up to about 10 000 (row < 71) */

seq_t a(idx_t n)
{
    /* arrays that hold the current and next rows of B(X,i)
       indices correspond to the exponent of X */
    static seq_t cur[MAX_DEG + 1];
    static seq_t next[MAX_DEG + 1];
    static seq_t diff[MAX_DEG];

    /* initialise the first polynomial B(X,1)=X^5 */
    for (size_t i = 0; i <= MAX_DEG; ++i)
        cur[i] = 0;
    cur[5] = (seq_t)1;         /* coefficient of X^5 */

    if (n == 1)
        return cur[5];

    /* number of terms already produced (row counts start at 1) */
    idx_t produced = 1;        /* a(1) is already counted */

    /* iterate over rows i = 2, 3, … until we reach the desired term */
    for (idx_t i = 2; ; ++i)
    {
        /* degrees of the current and next polynomials */
        size_t prevDeg = 4 * (i - 1) + 1;   /* B(X,i-1) has degree prevDeg */
        size_t nextDeg = 4 * i + 1;         /* B(X,i)   has degree nextDeg */

        /* clear the next array */
        for (size_t k = 0; k <= nextDeg; ++k)
            next[k] = 0;

        /* compute the difference polynomial C(X) = B(X+1,i-1) – B(X,i-1) */
        for (idx_t k = 0; k < (idx_t)prevDeg; ++k)
        {
            seq_t sum = 0;
            /* sum_{r=k}^{prevDeg} cur[r] * binom(r,k) */
            for (idx_t r = k; r <= (idx_t)prevDeg; ++r)
                sum += cur[r] * binom((idx_t)r, (idx_t)k);
            /* subtract the term from B(X,i-1) itself */
            sum -= cur[k];
            diff[k] = sum;
        }

        /* shift the difference by multiplying with X^5 */
        for (idx_t k = 0; k < (idx_t)prevDeg; ++k)
            next[k + 5] = diff[k];

        /* number of entries in this new row */
        idx_t rowLen = 4 * i - 3;   /* 4i-3 entries in row i */

        /* if the desired term lies in this row, compute its value */
        if (produced + rowLen >= n)
        {
            idx_t offset = (idx_t)(n - produced - 1); /* zero‑based offset */
            return next[5 + offset];
        }

        /* prepare for the next iteration */
        for (size_t k = 0; k <= nextDeg; ++k)
            cur[k] = next[k];
        produced += rowLen;
    }
}
