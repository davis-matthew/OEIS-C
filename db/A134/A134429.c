/*
 * A134429  a(n) = (-1)^(1 + n) * 2 * ( -1/2 -n + (-1)^((1 + n)*(2 + n)/2)).
 * https://oeis.org/A134429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (-1)^(1+n) * 2 * ( -1/2 - n + (-1)^((1 + n)*(2 + n)/2))
 * Range:     i8 n <= 63, i16 n <= 8441, i32 n <= 8441, i64 n <= 8441,
 *            i128 n <= 8441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned n_mod4 = n & 3U;                     /* n modulo 4 */
    seq_t t = (n_mod4 < 2U) ? (seq_t)-1 : (seq_t)1;   /* (-1)^((1+n)(2+n)/2) */
    seq_t sgn = (n & 1U) ? (seq_t)1 : (seq_t)-1;     /* (-1)^(1+n) */
    seq_t inside = -(seq_t)1 - (seq_t)2 * (seq_t)n + (seq_t)2 * t;
    return sgn * inside;
}
