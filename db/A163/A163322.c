/*
 * A163322  The 3rd Hermite Polynomial evaluated at n: H_3(n) = 8*n^3 - 12*n.
 * https://oeis.org/A163322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8*n^3 - 12*n
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 645, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider intermediate type to avoid overflow in the cubic calculation. */
    __int128 m   = ( __int128 ) n;
    __int128 val = 8 * m * m * m - 12 * m;
    return (seq_t) val;
}
