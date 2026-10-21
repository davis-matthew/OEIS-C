/*
 * A085622  Maximal number of segments (equivalently, corners) in a rook
 *          circuit of a 2n X 2n board.
 * https://oeis.org/A085622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 - 2*n (n even, n>1) = 4*n^2 - 2*n - 2 (n odd, n>1)
 *            a(0) = 1, a(1) = 4
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    seq_t nt = (seq_t)n;
    seq_t result = 4 * nt * nt - 2 * nt;
    if ((n & 1U) != 0) /* n is odd */
        result -= 2;
    return result;
}
