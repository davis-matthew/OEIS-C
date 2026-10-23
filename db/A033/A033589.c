/*
 * A033589  a(n) = (2*n-1)*(3*n-1)*(4*n-1).
 * https://oeis.org/A033589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n-1)*(3*n-1)*(4*n-1)
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 447, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = 2 * sn - 1;
    seq_t t2 = 3 * sn - 1;
    seq_t t3 = 4 * sn - 1;
    return t1 * t2 * t3;
}
