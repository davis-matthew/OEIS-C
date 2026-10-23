/*
 * A047810  Duplicate of A002492.
 * https://oeis.org/A047810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*n*(n+1)*(2*n+1)/3
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num = sn * (sn + 1);
    num *= (seq_t)(2 * sn + 1);
    num *= 2;
    return num / 3;
}
