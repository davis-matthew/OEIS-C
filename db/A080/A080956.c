/*
 * A080956  a(n) = (n+1)*(2-n)/2.
 * https://oeis.org/A080956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)*(2-n)/2
 * Range:     i8 n <= 16, i16 n <= 256, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use seq_t for arithmetic to preserve sign and range */
    seq_t nn = (seq_t)n;           /* cast unsigned index to signed type */
    return ((nn + 1) * (2 - nn)) / 2;
}
