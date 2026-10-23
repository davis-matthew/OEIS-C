/*
 * A157737  a(n) = 441*n^2 - 2*n.
 * https://oeis.org/A157737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n*n - 2*n
 * Range:     u8 none, u16 n <= 12, u32 n <= 3120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)441 * nn * nn - ( (seq_t)2 * nn );
    return res;
}
