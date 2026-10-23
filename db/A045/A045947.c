/*
 * A045947  Triangles in open triangular matchstick arrangement (triangle
 *          minus one side) of side n.
 * https://oeis.org/A045947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/16)*(2*n*(2*n^2 + n - 2)+(-1)^n-1)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1110, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;      /* cast index to seq_t */
    seq_t n2  = N * N;         /* n^2 */
    seq_t n3  = N * n2;        /* n^3 */

    seq_t num = (seq_t)4 * n3 + (seq_t)2 * n2 - (seq_t)4 * N;

    /* (-1)^n - 1 is 0 for even n and -2 for odd n */
    if (n & (seq_t)1)          /* odd n */
        num -= (seq_t)2;

    return num / (seq_t)16;
}
