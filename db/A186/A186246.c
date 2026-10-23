/*
 * A186246  (2n+1)-th derivative of arccot(x) at x=0.
 * https://oeis.org/A186246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * (2n)!
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute factorial of 2n */
    seq_t limit = ((seq_t)n) * ((seq_t)2);
    seq_t r = 1;
    for (seq_t i = 2; i <= limit; ++i) {
        r *= i;
    }
    /* apply alternating sign: even n -> negative, odd n -> positive */
    if ((n & 1) == 0) {          /* n even */
        return -r;
    } else {                     /* n odd */
        return r;
    }
}
