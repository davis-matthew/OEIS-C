/*
 * A036409  a(n) = ceiling(n^2/11).
 * https://oeis.org/A036409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/11)
 * Range:     u8 n <= 52, u16 n <= 849, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before performing multiplication to avoid overflow
       of the intermediate into idx_t. */
    seq_t nn = (seq_t)n;
    return (nn * nn + 10) / 11;
}
