/*
 * A131738  a(0) = 0. a(n) = (n+1)*(-1)^n, n>0 .
 * https://oeis.org/A131738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0) = 0, a(n) = (n+1)*(-1)^n for n > 0
 * Range:     i8 n <= 127, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t val = (seq_t)n + 1;   /* compute n+1 in the result type */
    if (n & 1)                  /* if n is odd, flip the sign */
        val = -val;
    return val;
}
