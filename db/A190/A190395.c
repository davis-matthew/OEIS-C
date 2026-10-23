/*
 * A190395  Number of ways to place 3 nonattacking grasshoppers on a
 *          chessboard of size n x n.
 * https://oeis.org/A190395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/6 * (n-1) * (n^5 + n^4 - 2*n^3 - 22*n^2 + 76*n - 72)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset; for n <= 1 the answer is 0 */
    if (n <= 1)
        return (seq_t)0;

    /* Convert the index to seq_t to avoid intermediate overflow */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;

    /* Compute the positive part and the negative part of the polynomial
       separately to avoid unsigned underflow. The inequality
       N^5 + N^4 + 76*N >= 2*N^3 + 22*N^2 + 72 holds for all n >= 2. */
    seq_t pos = N5 + N4 + (seq_t)76 * N;
    seq_t neg = (seq_t)2 * N3 + (seq_t)22 * N2 + (seq_t)72;
    seq_t poly = pos - neg;        /* polynomial part, non‑negative */

    /* Multiply by (n-1) and divide by 6 (exact division). */
    seq_t num = (N - (seq_t)1) * poly;
    seq_t res = num / (seq_t)6;

    return res;
}
