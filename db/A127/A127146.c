/*
 * A127146  Q(n,4), where Q(m,k) is defined in A127080 and A127137.
 * https://oeis.org/A127146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 10*n + 12
 * Range:     i8 n <= 16, i16 n <= 186, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return tn * tn - (seq_t)10 * tn + (seq_t)12;
}
