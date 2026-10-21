/*
 * A202785  Number of 3 X 3 0..n arrays with row and column sums equal.
 * https://oeis.org/A202785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^5 + 15*n^4 + 35*n^3 + 45*n^2 + 32*n + 10) / 10
 *            (empirically verified; equivalent to OEIS A202785)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used; safety */
    seq_t nn = (seq_t)n;            /* n as seq_t */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t num = (seq_t)3 * n5
              + (seq_t)15 * n4
              + (seq_t)35 * n3
              + (seq_t)45 * n2
              + (seq_t)32 * nn
              + (seq_t)10;
    return num / (seq_t)10;
}
