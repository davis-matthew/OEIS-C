/*
 * A102400  Triangle, read by rows, where T(n,k) = Sum_{j=0..k}
 *          T(n-1,j)*(j+1)*[(k+1)*(k+2)/2 - j*(j+1)/2] for n>k>0, with
 *          T(0,0)=1 and T(n,n) = T(n,n-1) for n>0.
 * https://oeis.org/A102400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A102400: Triangle read by rows
 *
 * Definition:
 *   T(0,0) = 1
 *   T(n,0) = 1                               for n > 0
 *   T(n,n) = T(n,n-1)                        for n > 0
 *   T(n,k) = sum_{j=0..k} T(n-1,j)*(j+1) * 
 *           [ (k+1)*(k+2)/2 - j*(j+1)/2 ]    for n > k > 0
 *
 * Sequence a is the triangle written row by row:
 *   a(0) = T(0,0)
 *   a(1) = T(1,0), a(2) = T(1,1)
 *   a(3) = T(2,0), a(4) = T(2,1), a(5) = T(2,2)
 *   ...
 *
 * The function below returns a(n) for any n ≥ 0.
 * It recomputes rows up to the required one; the needed row index
 * is only up to about 140 for n ≤ 10000, so the routine is O(r²)
 * with r ≈ √(2n).
 */

#include <stddef.h> /* for NULL, size_t if needed */

#define MAX_R 200 /* > maximum row needed for n ≤ 10⁴ (round(√(2·10⁴))≈141) */

seq_t a(idx_t n)
{
    /* Determine row r and column k such that
       a(n) = T(r,k) with start index of row r = r(r+1)/2. */
    idx_t r = 0;
    idx_t cum = 0;            /* cumulative count up to previous row      */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;        /* column within row r */

    /* Special case r = 0 */
    if (r == 0) {
        return 1; /* T(0,0) */
    }

    /* Allocate two static rows: previous and current */
    static seq_t prevRow[MAX_R];
    static seq_t curRow[MAX_R];

    /* Initialize row 0 */
    prevRow[0] = 1;

    /* Build rows successively up to row r */
    for (idx_t row = 1; row <= r; ++row) {
        curRow[0] = 1; /* T(row,0) = 1 */

        for (idx_t kk = 1; kk < row; ++kk) { /* compute T(row,kk) for kk < row */
            seq_t sum = 0;
            /* triangular number for k */
            seq_t tri_k = (seq_t)(kk + 1) * (seq_t)(kk + 2) / 2L;

            for (idx_t j = 0; j <= kk; ++j) {
                seq_t tri_j = (seq_t)j * (seq_t)(j + 1) / 2L; /* j*(j+1)/2 */
                seq_t R    = tri_k - tri_j;                  /* R(j,kk)   */
                seq_t term = prevRow[j] * (seq_t)(j + 1) * R;
                sum += term;
            }
            curRow[kk] = sum;
        }

        /* Diagonal element */
        curRow[row] = curRow[row - 1]; /* T(row,row) = T(row,row-1) */

        /* Copy current row to prevRow for next iteration */
        for (idx_t i = 0; i <= row; ++i) {
            prevRow[i] = curRow[i];
        }
    }

    return prevRow[k];
}
