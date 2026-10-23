/*
 * A039763  Triangle of D-analogs of Stirling numbers of first kind, rows
 *          reversed.
 * https://oeis.org/A039763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 31, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = flattening of the triangle of D-analogs of Stirling
 * numbers of first kind.  For each row r (r ≥ 0) the r-th row contains
 * r+1 terms.  The r-th row is given by the coefficients of the polynomial
 *
 *   P_r(x) = (x-(r-1)) * ∏_{k=1}^{r-1} (x-(2*k-1))
 *
 * where the coefficient of x^(r-k) equals a(n) for the k-th element of
 * that row.  The function finds the correct row and position for the
 * requested index n and returns the corresponding coefficient. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* find row r such that sum_{i=0}^{r-1} (i+1) <= n < sum_{i=0}^{r} (i+1) */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }
    /* start index of this row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;            /* 0 ≤ k ≤ r */

    if (r == 0) {
        /* first row has a single entry equal to 1 */
        return (seq_t)1;
    }

    /* allocate an array for the polynomial coefficients */
    seq_t *coeff = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (coeff == NULL) {
        /* out of memory – return 0 as a safe fallback */
        return (seq_t)0;
    }
    coeff[0] = (seq_t)1;  /* start with polynomial = 1 */

    idx_t deg = 0;  /* current degree of polynomial */

    /* first root a = r-1 */
    seq_t a_root = (seq_t)(r - 1);
    {
        seq_t old0 = coeff[0];
        for (idx_t j = deg; (long long)j >= 0; --j) {
            coeff[j + 1] = coeff[j] - a_root * coeff[j + 1];
        }
        coeff[0] = -a_root * old0;
        deg++;
    }

    /* remaining roots: odd numbers 1,3,5,...,2*(r-1)-1   (r-1 of them) */
    for (idx_t m = 1; m <= r - 1; ++m) {
        seq_t a = (seq_t)(2 * m - 1);
        seq_t old0 = coeff[0];
        for (idx_t j = deg; (long long)j >= 0; --j) {
            coeff[j + 1] = coeff[j] - a * coeff[j + 1];
        }
        coeff[0] = -a * old0;
        deg++;
    }

    /* coefficient of x^(r-k) is at index r-k */
    seq_t result = coeff[r - k];
    free(coeff);
    return result;
}
