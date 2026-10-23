/*
 * A038608  a(n) = n*(-1)^n.
 * https://oeis.org/A038608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(-1)^n
 * Range:     i8 n <= 127, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;   /* promote n to seq_t */
    if (n & 1u)            /* if n is odd */
        val = -val;
    return val;
}
