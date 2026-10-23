/*
 * A127148  Q(n,6), where Q(m,k) is defined in A127080 and A127137.
 * https://oeis.org/A127148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 -24*n^2 +128*n -120
 * Range:     i8 n <= 8, i16 n <= 40, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn*sn*sn - (seq_t)24*sn*sn + (seq_t)128*sn - (seq_t)120;
}
