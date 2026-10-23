/*
 * A190398  Number of ways to place 3 nonattacking grasshoppers on a toroidal
 *          chessboard of size n x n.
 * https://oeis.org/A190398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/6*n^2*(n^4 - 3*n^2 - 24*n + 74) for n>3; special cases:
 *            a(1)=0, a(2)=4, a(3)=72
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)72;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;                   /* n^2 */
    seq_t n4  = n2 * n2;                 /* n^4 */
    seq_t inner = n4
                  - (seq_t)3 * n2
                  - (seq_t)24 * N
                  + (seq_t)74;           /* n^4 - 3*n^2 - 24*n + 74 */
    seq_t prod  = n2 * inner;            /* n^2 * inner */
    seq_t result = prod / (seq_t)6;      /* divide by 6 */
    return result;
}
