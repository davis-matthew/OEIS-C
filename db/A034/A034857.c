/*
 * A034857  a(n) = C(n+2,3) + 2*C(n,2) + 2*(n-2).
 * https://oeis.org/A034857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^3 + 9n^2 + 8n - 24)/6
 * Range:     i8 n <= 3, i16 n <= 29, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t term = tn * tn * tn
               + (seq_t)9 * tn * tn
               + (seq_t)8 * tn
               - (seq_t)24;
    return term / (seq_t)6;
}
