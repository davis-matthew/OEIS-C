/*
 * A162258  a(n) = (2*n^3 + 5*n^2 - 9*n)/2.
 * https://oeis.org/A162258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 9*n)/2
 * Range:     i8 n <= 3, i16 n <= 24, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;        /* Convert index to seq_t once */
    seq_t sq  = sn * sn;         /* n^2 */
    seq_t cube= sq * sn;         /* n^3 */
    seq_t num = (seq_t)2 * cube + (seq_t)5 * sq - (seq_t)9 * sn;
    return num / (seq_t)2;       /* Integer division, exact by formula */
}
