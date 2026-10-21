/*
 * A079273  Octo numbers (a polygonal sequence): a(n) = 5*n^2 - 6*n + 2 =
 *          (n-1)^2 + (2*n-1)^2.
 * https://oeis.org/A079273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 5*n^2 - 6*n + 2
 * Range:     u8 n <= 7, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 5 * nn * nn;
    val -= 6 * nn;
    val += 2;
    return val;
}
