/*
 * A195233  Number of lower triangles of a 3 X 3 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by one or less.
 * https://oeis.org/A195233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 91*n - 27
 * Range:     u8 n <= 3, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (seq_t)91 - (seq_t)27;
    return res;
}
