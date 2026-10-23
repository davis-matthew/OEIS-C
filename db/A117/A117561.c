/*
 * A117561  a(n) = floor(n*(n^3-n-3)/(2*(n-1))).
 * https://oeis.org/A117561
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n*(n^3 - n - 3) / (2*(n-1)))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t numerator = nn * (n3 - nn - (seq_t)3);
    seq_t denominator = (seq_t)2 * (nn - (seq_t)1);
    return numerator / denominator;
}
