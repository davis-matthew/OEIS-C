/*
 * A157155  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) - m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 4, read by rows.
 * https://oeis.org/A157155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Triangle T(n_row, k_col, 4) flattened row‑major T(n,k,m)
 *            = (m*(n-k)+1)*T(n-1,k-1,m) + (m*k+1)*T(n-1,k,m) -
 *            m*k*(n-k)*T(n-2,k-1,m) with T(n,0,m)=T(n,n,m)=1 m = 4
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 57, u64 n <= 140,
 *            u128 n <= 360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Find the row r that contains the linear index n
       (row r starts at index r*(r+1)/2). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t col = n - r * (r + 1) / 2;

    seq_t *row_prev2 = NULL;   /* row i‑2   */
    seq_t *row_prev1 = NULL;   /* row i‑1   */
    seq_t *row_cur   = NULL;   /* row i     */

    for (idx_t i = 0; i <= r; ++i) {
        size_t len = (size_t)(i + 1);

#ifdef __cplusplus
        row_cur = new seq_t[len];
#else
        row_cur = (seq_t *)malloc(sizeof(seq_t) * len);
#endif
        for (idx_t k = 0; k < len; ++k) {
            if (k == 0 || k == i) {          /* borders */
                row_cur[k] = 1;
            } else {
                /* (4*(i-k)+1)*T(i-1,k-1)  */
                seq_t term1 = ((seq_t)4 * (seq_t)(i - k) + 1);
                term1 *= row_prev1[k - 1];

                /* (4*k+1)*T(i-1,k)        */
                seq_t term2 = ((seq_t)4 * (seq_t)k + 1);
                term2 *= row_prev1[k];

                /* 4*k*(i-k)*T(i-2,k-1)    */
                seq_t term3 = ((seq_t)4 * (seq_t)k * (seq_t)(i - k));
                term3 *= row_prev2[k - 1];

                row_cur[k] = term1 + term2 - term3;
            }
        }

        /* Shift the rows for the next iteration */
#ifdef __cplusplus
        delete[] row_prev2;
#else
        free(row_prev2);
#endif
        row_prev2 = row_prev1;
        row_prev1 = row_cur;
    }

    /* Result is the element at column col of the final row */
    seq_t result = row_prev1[col];

    /* Clean up memory */
#ifdef __cplusplus
    delete[] row_prev2;
    delete[] row_prev1;
#else
    free(row_prev2);
    free(row_prev1);
#endif

    return result;
}
