/*
 * A175869  Partial sums of ceiling(n^2/23).
 * https://oeis.org/A175869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + 85*n + 90)/138)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 900, u64 n <= 900,
 *            u128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This is equivalent to the OEIS definition of A175869:   */
/*  partial sums of ceil(n^2/23) with offset 0.             */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t num = (seq_t)2 * n3 + (seq_t)3 * n2 + (seq_t)85 * N + (seq_t)90;
    return num / (seq_t)138;   /* integer division = floor */
}
