/*
 * A015116  Triangle of q-binomial coefficients for q=-6.
 * https://oeis.org/A015116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = Gaussian q‑binomial coefficient evaluated at q = -6, read
 *            from the triangle of coefficients along antidiagonals
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 30, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row and column in the triangle from the antidiagonal
       index n.  The cumulative number of elements before row r is r(r+1)/2. */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;
    idx_t col = n - row * (row + 1) / 2;   /* 0 ≤ col ≤ row */

    /* The q‑binomial coefficient is 1 when col = 0 or col = row. */
    if (col == 0 || col == row)
        return (seq_t)1;

    /* Pre‑compute powers of q = -6 up to exponent row. */
    const seq_t Q = -((seq_t)6);
    seq_t *pow_arr = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    if (!pow_arr)
        return (seq_t)0;   /* unlikely for small row, but defensive */

    pow_arr[0] = (seq_t)1;
    for (idx_t e = 1; e <= row; ++e)
        pow_arr[e] = pow_arr[e - 1] * Q;

    /* Compute the q‑binomial coefficient multiplicatively. */
    seq_t result = (seq_t)1;
    const seq_t one = (seq_t)1;
    for (idx_t i = 0; i < col; ++i) {
        seq_t num = one - pow_arr[row - i];
        seq_t den = one - pow_arr[i + 1];
        result = result * num / den;   /* exact division */
    }

    free(pow_arr);
    return result;
}
