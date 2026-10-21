/*
 * A057769  a(n) = 4*n^4 + 8*n^3 - 4*n - 1 = (2*n^2 - 1)*(2*n^2 + 4*n + 1).
 * https://oeis.org/A057769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2*n^2 - 1)*(2*n^2 + 4*n + 1)
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 151, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;        /* convert unsigned index to seq_t   */
    seq_t n2  = sn * sn;         /* n^2                                 */
    seq_t t1  = 2 * n2 - 1;      /* 2*n^2 - 1                           */
    seq_t t2  = 2 * n2 + 4 * sn + 1;  /* 2*n^2 + 4*n + 1                */
    return t1 * t2;              /* product gives a(n)                   */
}
