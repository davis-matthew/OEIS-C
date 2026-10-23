/*
 * A154599  a(n) = 2*n^2 + 20*n + 8.
 * https://oeis.org/A154599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 20*n + 8
 * Range:     u8 n <= 7, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                      /* cast index to seq_t for safety   */
    return 2 * nn * nn + 20 * nn + 8;  /* direct evaluation of the formula */
}
