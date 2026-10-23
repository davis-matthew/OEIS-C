/*
 * A140755  Square array read by rows: T(n,k) = 41*n^2 + k^2 + n*k, with 20
 *          columns.
 * https://oeis.org/A140755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   A140755 – square array read by rows. a(n) = 41*r^2 + c^2 + r*c
 *            where r = (n-1)/20 + 1 (row index) c = (n-1)%20 + 1 (column
 *            index)
 * Range:     u8 n <= 14, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the 1‑based index to row (r) and column (c). */
    idx_t m = n - 1;          /* unsigned subtraction; n ≥ 1 */
    seq_t r = (seq_t)(m / 20) + 1;   /* row number */
    seq_t c = (seq_t)(m % 20) + 1;   /* column number */

    /* Compute the array value. */
    seq_t val = 41 * r * r + c * c + r * c;
    return val;
}
