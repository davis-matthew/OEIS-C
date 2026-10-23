/*
 * A135855  A007318 * a tridiagonal matrix with (1, 4, 1, 0, 0, 0, ...) in
 *          every column.
 * https://oeis.org/A135855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k) * ( n^2 + (2*k+7)*n - 2*(k^2 + 2*k -1) )
 *            / ((k+1)*(k+2)), T(n,0) = 1 + n(n+7)/2; a(n) is the triangle
 *            read row‑by‑row (row 0 first, then row 1, …)
 * Range:     u8 n <= 37, u16 n <= 141, u32 n <= 449, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

/* Computes the index (row r, column c) of the flattened triangle element n */
static void row_col(idx_t n, idx_t *r, idx_t *c)
{
    /* Convert to double to find an initial estimate of the row.
       n is assumed to be small enough that 8*n+1 fits in double without rounding errors
       (true for n up to several billions). */
    double sd = sqrt(8.0 * (double)n + 1.0);
    idx_t r_est = (idx_t)((sd - 3.0) / 2.0) + 1;   /* approximation for r */

    /* Correct possible off‑by‑one caused by floating point error */
    idx_t s_prev = (r_est * (r_est + 1)) / 2;      /* S(r_est-1) = r_est*(r_est+1)/2 */
    while (s_prev > n) {
        r_est--;
        s_prev = (r_est * (r_est + 1)) / 2;
    }
    while ((s_prev + (r_est + 1)) <= n) {
        r_est++;
        s_prev = (r_est * (r_est + 1)) / 2;
    }

    *r = r_est;
    *c = n - s_prev;
}

seq_t a(idx_t n)
{
    idx_t r, c;
    row_col(n, &r, &c);

    /* Value on the main diagonal */
    if (c == r)
        return (seq_t)1;

    /* First column value: 1 + r(r+7)/2 */
    if (c == 0) {
        seq_t r_s = (seq_t)r;
        return (seq_t)1 + r_s * (r_s + (seq_t)7) / (seq_t)2;
    }

    /* General case: use the explicit formula */
    seq_t bin = binom(r, c);

    seq_t r_s = (seq_t)r;
    seq_t c_s = (seq_t)c;

    seq_t num = r_s * r_s               /* n^2 */
              + (seq_t)2 * c_s * r_s    /* (2k+7)n part will be added later */
              + (seq_t)7 * r_s
              - ((seq_t)2 * c_s * c_s
                 + (seq_t)4 * c_s
                 - (seq_t)2);

    seq_t denom = (c_s + (seq_t)1) * (c_s + (seq_t)2);

    return bin * num / denom;
}
