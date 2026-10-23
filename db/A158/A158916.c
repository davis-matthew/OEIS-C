/*
 * A158916  Inverse binomial transform of A153130.
 * https://oeis.org/A158916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) - 3*a(n-2) - 2*a(n-3) for n > 3 Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2 || n == 3) return (seq_t)1;
    seq_t t1 = (seq_t)1;   /* a(0)   */
    seq_t t2 = (seq_t)1;   /* a(1)   */
    seq_t t3 = (seq_t)1;   /* a(2)   */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t4 = -(seq_t)3 * t3 - (seq_t)3 * t2 - (seq_t)2 * t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
    }
    return t3;              /* a(n)   */
}
