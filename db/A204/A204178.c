/*
 * A204178  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of (f(i,j)), where
 *          f(i,j)=(1 if i=1 or j=1 or i=j, and 0 otherwise) as in A204177.
 * https://oeis.org/A204178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(k) is the coefficient of λ^{pos} in the characteristic *
 *            polynomial of the r-th principal submatrix of the matrix *
 *            f(i,j)=1 if i=1 or j=1 or i=j, otherwise 0. * The polynomial is
 *            * * P_r(λ) = det(A_r - λI) = (1-λ)^{r-2} * ((1-λ)^2 - (r-1)) * *
 *            The sequence is obtained by writing the coefficien...
 * Range:     i8 n <= 46, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: s(r) = cumulative number of terms up to row r
 * s(r) = r*(r+3)/2  (integer arithmetic; use 128-bit to avoid overflow)
 */
static inline __int128 row_sum(idx_t r)
{
    return (__int128)r * (r + 3) / 2;
}

/* Main function: return the k-th term of the sequence */
seq_t a(idx_t idx)
{
    /* Find the row r such that s(r-1) < idx <= s(r)  */
    idx_t low = 0, high = 1;
    while (row_sum(high) < (__int128)idx)
        high *= 2;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (row_sum(mid) >= (__int128)idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number (>=1)                */
    idx_t prev_total = (idx_t)row_sum(r - 1);
    idx_t pos = idx - prev_total - 1;   /* 0‑based index within the row   */

    /* Handle the trivial first row separately */
    if (r == 1) {
        return (pos == 0) ? (seq_t)1 : (seq_t)-1;
    }

    /* Compute the binomial coefficients B_k = (-1)^k * C(r-2, k)   */
    idx_t m = r - 2;                    /* last index of B[]                 */
    seq_t *B = (seq_t *)malloc((r - 1) * sizeof(seq_t));
    if (!B) return (seq_t)0;            /* safety fallback                 */
    B[0] = 1;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t factor = (seq_t)(- (int64_t)(m - (k - 1))); /* -(m-(k-1)) */
        B[k] = (B[k - 1] * factor) / (seq_t)k;
    }

    /* Compute the desired coefficient: */
    seq_t coeff = 0;
    seq_t two_minus_r = (seq_t)(2 - (int64_t)r);

    if (pos <= m)
        coeff += two_minus_r * B[pos];
    if (pos >= 1 && pos - 1 <= m)
        coeff += (seq_t)-2 * B[pos - 1];
    if (pos >= 2 && pos - 2 <= m)
        coeff += B[pos - 2];

    free(B);
    return coeff;
}
