/*
 * A127365  Signed repeated natural numbers.
 * https://oeis.org/A127365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(n/2) * (-1)^floor(n/2)
 * Range:     i8 n <= 255, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t res = (seq_t)k;       /* cast index to seq_t */
    if (k & 1) res = -res;      /* apply sign (-1)^k when k is odd */
    return res;
}
