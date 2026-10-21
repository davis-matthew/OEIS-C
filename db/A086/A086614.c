/*
 * A086614  Triangle read by rows, where T(n,k) is the coefficient of x^n*y^k
 *          in f(x,y) that satisfies f(x,y) = 1/(1-x)^2 + xy*f(x,y)^2.
 * https://oeis.org/A086614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r is the row such that r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, c = n - r*(r+1)/2. T(r,0) = r+1. For k>0: T(r,k)
 *            = binomial(2*k, k-1) * binomial(r+k+1, r-k) / k
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides binomial(idx_t, idx_t) -> seq_t */

/* Compute the triangle entry T(n,k). */
static seq_t triangle_value(idx_t n, idx_t k)
{
    if (k == 0)                     /* first column: T(n,0)=n+1 */
        return (seq_t)(n + 1);

    seq_t b1 = binomial(2 * k, k - 1);          /* binomial(2k, k-1)   */
    seq_t b2 = binomial(n + k + 1, n - k);      /* binomial(n+k+1, n-k)*/
    return (b1 * b2) / (seq_t)k;                /* division is exact   */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r and column c of the term a(n).   */
    idx_t start = 0;                /* index of the first element in the current row   */
    idx_t row   = 0;                /* current row index                              */

    while (1) {
        /* length of this row is row + 1 */
        idx_t row_len = row + 1;

        if (n < start + row_len) {          /* the n-th element is in this row   */
            idx_t col = n - start;          /* column index (0‑based)             */
            return triangle_value(row, col);
        }
        /* move to the next row */
        start += row_len;
        ++row;
    }
}
