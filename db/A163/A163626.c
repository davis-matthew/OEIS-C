/*
 * A163626  Triangle read by rows: The n-th derivative of the logistic
 *          function written in terms of y, where y = 1/(1 + exp(-x)).
 * https://oeis.org/A163626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,j) where T(0,0)=1,
 *            T(n,k)=(k+1)*T(n-1,k)-k*T(n-1,k-1), r is the row and j is the
 *            column such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 and j = n -
 *            r*(r+1)/2
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 83, i64 n <= 200,
 *            i128 n <= 517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* find row r and column j for flattened index n */
    idx_t r = 0;
    idx_t cum = 0;               /* number of elements before row r */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t j = n - cum;

    /* Allocate array for row 0 */
#ifdef __cplusplus
    seq_t *prev = new seq_t[r + 1];
#else
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
#endif
    if (!prev) return 0;        /* allocation failed */

    prev[0] = 1;                /* T(0,0) = 1 */

    for (idx_t cur = 1; cur <= r; ++cur) {
#ifdef __cplusplus
        seq_t *curRow = new seq_t[cur + 1];
#else
        seq_t *curRow = (seq_t *)malloc((cur + 1) * sizeof(seq_t));
#endif
        if (!curRow) {
#ifdef __cplusplus
            delete[] prev;
#else
            free(prev);
#endif
            return 0;
        }

        curRow[0] = 1;            /* T(cur,0) = 1 */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = 0;
            if (k <= cur - 1)
                term1 = prev[k] * (seq_t)(k + 1);
            seq_t term2 = prev[k - 1] * (seq_t)k;
            curRow[k] = term1 - term2;
        }

#ifdef __cplusplus
        delete[] prev;
#else
        free(prev);
#endif
        prev = curRow;
    }

    seq_t result = prev[j];

#ifdef __cplusplus
    delete[] prev;
#else
    free(prev);
#endif
    return result;
}
