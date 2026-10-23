/*
 * A188652  First differences of A000463.
 * https://oeis.org/A188652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+3-(2*n^2-2*n-5)*(-1)^n)/8
 * Range:     i8 n <= 8, i16 n <= 236, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type. */
    seq_t nn = (seq_t)n;

    /* Parity factor: (-1)^n, 1 for even n, -1 for odd n. */
    seq_t parity = ((n & 1) ? (seq_t)-1 : (seq_t)1);

    /* Compute the polynomial part: 2*n^2 - 2*n - 5. */
    seq_t poly = 2 * nn * nn - 2 * nn - 5;

    /* Multiply by (-1)^n. */
    poly *= parity;

    /* Final expression: (2*n + 3 - poly) / 8. */
    seq_t result = (2 * nn + 3 - poly) / 8;

    return result;
}
