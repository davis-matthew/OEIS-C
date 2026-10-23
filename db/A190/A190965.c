/*
 * A190965  a(n) = 4*a(n-1) - 6*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 49, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)4 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
