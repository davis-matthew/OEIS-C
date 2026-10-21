/*
 * A202254  Number of zero-sum -n..n arrays of 4 elements with adjacent
 *          element differences also in -n..n.
 * https://oeis.org/A202254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (22*n^3 + 33*n^2 + 26*n + (n%2?3:12)) / 12
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = (seq_t)22 * n3 + (seq_t)33 * n2 + (seq_t)26 * nn;
    if ((n & 1) == 0)
        num += (seq_t)12;
    else
        num += (seq_t)3;
    return num / (seq_t)12;
}
