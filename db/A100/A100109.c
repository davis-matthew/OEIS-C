/*
 * A100109  a(n) = n^3 - 2*n^2 + 2.
 * https://oeis.org/A100109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - 2*n^2 + 2
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;          /* cast index to seq_t */
    return nn*nn*nn - 2*nn*nn + 2;
}
