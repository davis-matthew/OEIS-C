/*
 * A143859  Ulam's spiral (WNW spoke).
 * https://oeis.org/A143859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 31*n + 16
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    seq_t t = N * N;   /* N^2 */
    t *= 16;           /* 16*N^2 */
    return t - 31 * N + 16;
}
