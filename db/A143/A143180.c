/*
 * A143180  Triangle read by rows: T(n, k) = (n-k+1)^3 + (k+1)^3 -
 *          3*(n-k+1)*(k+1).
 * https://oeis.org/A143180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the linearized reading of * the triangle T(r,k) =
 *            (r-k+1)^3 + (k+1)^3 - 3*(r-k+1)*(k+1) * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     i8 n <= 14, i16 n <= 527, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n)
{
    /* find row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n )
        ++r;

    /* position within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* compute values in seq_t to avoid overflow */
    seq_t A = (seq_t)(r - k + 1);
    seq_t B = (seq_t)(k + 1);

    seq_t a_val = A * A * A + B * B * B - 3 * A * B;

    return a_val;
}
