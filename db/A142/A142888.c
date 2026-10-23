/*
 * A142888  First differences of A142705.
 * https://oeis.org/A142888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3*n^2+8*n+1)/4 for n odd; a(n) = (-3*n^2+2*n+4)/4 for n
 *            even
 * Range:     i8 n <= 12, i16 n <= 208, i32 n <= 1001, i64 n <= 1001,
 *            i128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0)               /* n is even */
    {
        /* a(n) = (-3*n^2 + 2*n + 4) / 4 */
        return ((seq_t)(-3) * sn * sn + (seq_t)2 * sn + (seq_t)4) / (seq_t)4;
    }
    else                            /* n is odd */
    {
        /* a(n) = (3*n^2 + 8*n + 1) / 4 */
        return ((seq_t)3 * sn * sn + (seq_t)8 * sn + (seq_t)1) / (seq_t)4;
    }
}
