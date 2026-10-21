/*
 * A228219  Number of second differences of arrays of length 4 of numbers in
 *          0..n.
 * https://oeis.org/A228219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*n^2 + 4*n + 1
 * Range:     u8 n <= 4, u16 n <= 80, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)10 * nn * nn + (seq_t)4 * nn + (seq_t)1;
}
