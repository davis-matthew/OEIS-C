/*
 * A158667  a(n) = 841*n^2 - 29.
 * https://oeis.org/A158667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 841*n*n - 29
 * Range:     u8 none, u16 n <= 8, u32 n <= 2259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 841 * tn * tn - 29;
}
