/*
 * A008512  Number of points on the surface of 5-dimensional cube.
 * https://oeis.org/A008512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^4 + 20*n^2 + 2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before each operation to avoid overflow in idx_t */
    seq_t n2 = ((seq_t)n * (seq_t)n);      /* n^2   */
    seq_t n4 = n2 * n2;                   /* n^4   */
    return 10 * n4 + 20 * n2 + 2;
}
