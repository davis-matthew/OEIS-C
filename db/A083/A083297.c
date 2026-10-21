/*
 * A083297  a(n) = (4*4^n + (-6)^n)/5.
 * https://oeis.org/A083297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4*4^n + (-6)^n)/5
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)2;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)(-2) * a_nm1 + (seq_t)24 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
