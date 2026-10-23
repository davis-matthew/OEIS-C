/*
 * A157010  a(n) = 1681*n^2 - 756*n + 85.
 * https://oeis.org/A157010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1681*n^2 - 756*n + 85
 * Range:     u8 none, u16 n <= 6, u32 n <= 1598, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t to avoid intermediate signed arithmetic */
    seq_t nn = (seq_t)n;
    return (seq_t)1681 * nn * nn - (seq_t)756 * nn + (seq_t)85;
}
