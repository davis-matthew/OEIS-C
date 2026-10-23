/*
 * A118057  a(n) = 8*n^2 - 4*n - 3.
 * https://oeis.org/A118057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 4*n - 3
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    return 8 * nn * nn - 4 * nn - (seq_t)3;
}
