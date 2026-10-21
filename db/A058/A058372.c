/*
 * A058372  a(n) = -(n + 1)*(2*n^2 + n - 12)/6.
 * https://oeis.org/A058372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(n + 1)*(2*n^2 + n - 12)/6
 * Range:     i8 n <= 4, i16 n <= 24, i32 n <= 1859, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n1 = ns + 1;
    seq_t n2 = ns * ns;
    seq_t t = (2 * n2) + ns - 12;
    seq_t prod = -n1 * t;
    seq_t res = prod / 6;
    return res;
}
