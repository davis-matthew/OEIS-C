/*
 * A167573  a(n) = 20*n^2 + 3.
 * https://oeis.org/A167573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*n^2 + 3, offset 1
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 20*n^2 + 3 using only integer arithmetic. */
    return 20 * nn * nn + 3;
}
