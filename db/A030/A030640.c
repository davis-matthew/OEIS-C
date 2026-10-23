/*
 * A030640  Discriminant of lattice A_n of determinant n+1.
 * https://oeis.org/A030640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, changed
 * Formula:   a(2n) = (-1)^n*(2*n+1), a(2n+1) = (-1)^n*(n+1)
 * Range:     i8 n <= 62, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    unsigned parity = k & 1u;   /* k mod 2 */
    seq_t sign = (parity == 0u) ? 1 : -1;

    idx_t abs_val;
    if ((n & 1u) == 0u) {       /* n is even: a(n) = (-1)^k * (2k+1) = (-1)^k * (n+1) */
        abs_val = n + 1u;
    } else {                    /* n is odd:  a(n) = (-1)^k * (k+1) */
        abs_val = k + 1u;
    }

    return sign * (seq_t)abs_val;
}
