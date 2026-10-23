/*
 * A011864  a(n) = floor(n*(n - 1)/11).
 * https://oeis.org/A011864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/11)
 * Range:     u8 n <= 53, u16 n <= 849, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    return nn * (nn - 1) / 11;
}
