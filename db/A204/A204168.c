/*
 * A204168  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of (i+j), as in
 *          A003057.
 * https://oeis.org/A204168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the coefficient appearing in the row‑wise listing of the
 *            coefficients (constant term up to xⁿ) of det(A - xI) where A is
 *            the n×n matrix with entries aᵢⱼ = i + j
 * Range:     i8 n <= 32, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset starts at 1; safety */
    /* Find the row r that contains the n‑th flattened entry:
       r(r+3)/2 is the last index of row r. */
    idx_t r = (idx_t)floor((sqrt((double)9.0 + 8.0 * (double)n) - 3.0) / 2.0);
    while ((r * (r + 3)) / 2 < n) r++;

    /* Start index of this row (1‑based). */
    idx_t start = ((r - 1) * (r + 2)) / 2 + 1;
    idx_t pos = n - start;           /* 0‑based position inside the row */

    if (r == 1) {
        return pos == 0 ? (seq_t)2 : (seq_t)-1;
    }

    /* For r ≥ 2 the polynomial is:
       det(A - xI) = (-1)^{r-2}[C⋅x^{r-2} - S⋅x^{r-1} + x^r]
       with S = r(r+1) and C = -r²(r²-1)/12.
       The flattened list proceeds constant , x , … , x^r. */
    if (pos == r) {                    /* coefficient of x^r */
        seq_t sgn = ((r - 2) & 1) ? (seq_t)-1 : (seq_t)1;
        return sgn;
    }
    if (pos == r - 1) {                /* coefficient of x^{r-1} */
        seq_t S = (seq_t)r * (seq_t)(r + 1);
        seq_t sgn = ((r - 1) & 1) ? (seq_t)-1 : (seq_t)1;
        return sgn * S;
    }
    if (pos == r - 2) {                /* coefficient of x^{r-2} */
        seq_t r_sq = (seq_t)r * (seq_t)r;
        seq_t C = -(r_sq * (r_sq - (seq_t)1)) / (seq_t)12;
        seq_t sgn = ((r - 2) & 1) ? (seq_t)-1 : (seq_t)1;
        return sgn * C;
    }
    return (seq_t)0;                    /* zero for other positions (r ≥ 3) */
}
