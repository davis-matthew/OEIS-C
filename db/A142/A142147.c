/*
 * A142147  Irregular triangle read by rows: first row is 1, and the n-th row
 *          gives the coefficients in the expansion of (1/2*x)*(1 - 2*x*(1 -
 *          x))^(n + 1)*Li(-n, 2*x*(1 - x)), where Li(n, z) is the
 *          polylogarithm.
 * https://oeis.org/A142147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the coefficient of x^k in the polynomial P_r(x) = x^2
 *            (1-x) * E_r(2x(1-x)), where r is the row index determined from
 *            the flattened index n and k = pos+2. The Eulerian polynomial
 *            E_r(z) = Σ_{m=0}^{r-1} A(r,m) z^m uses the Eulerian numbers
 *            A(r,m). For n = 0 the value is 1
 * Range:     i8 n <= 22, i16 n <= 46, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   compute_eulerian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_eulerian.h"

/* Find the smallest r such that r*(r+1) > n-1 (i.e. the row containing n).  */
static inline idx_t row_of_index(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n - 1;
    idx_t r = 0;
    while (r * (r + 1) <= m) r++;       /* r grows at most O(√n)                     */
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* first row is 1 */

    /* Determine the row r and the position within that row.  */
    idx_t r  = row_of_index(n);                 /* r >= 1           */
    idx_t offset = 1 + r * (r - 1);             /* start index of row r  */
    idx_t pos   = n - offset;                   /* 0 <= pos < 2*r      */

    /* Compute Eulerian numbers A(r,m) for m = 0 .. r-1.  */
    seq_t *euler = NULL;
    size_t euler_len = 0;
    compute_eulerian(r, &euler, &euler_len);

    /* Accumulate the contribution to the coefficient at position 'pos'. */
    seq_t result   = (seq_t)0;
    seq_t pow2     = (seq_t)1;        /* 2^m for the current m  */

    for (size_t mi = 0; mi < euler_len; ++mi) {
        seq_t coeff   = euler[mi];
        seq_t bin_val = (seq_t)1;    /* binomial coefficient C(mi, i) */

        for (size_t i = 0; i <= mi; ++i) {
            seq_t term_common = coeff * pow2 * bin_val;
            int  sign = (i & 1) ? -1 : 1;
            idx_t idx2 = mi + i;          /* exponent of x in 2nd term */
            idx_t idx3 = idx2 + 1;        /* exponent of x in 3rd term */

            if (idx2 == pos) {
                if (sign > 0) result += term_common;
                else          result -= term_common;
            }

            if (idx3 == pos) {
                if (sign > 0) result -= term_common;
                else          result += term_common;
            }

            /* Next binomial coefficient C(mi, i+1) = C(mi, i) * (mi-i)/(i+1) */
            if (i < mi)
                bin_val = bin_val * (seq_t)(mi - i) / (seq_t)(i + 1);
        }

        /* Prepare 2^(mi+1) for the next m.  */
        pow2 += pow2;
    }

    free(euler);
    return result;
}
