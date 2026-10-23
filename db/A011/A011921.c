/*
 * A011921  [ n(n-1)(n-2)(n-3)/11 ].
 * https://oeis.org/A011921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/11)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1) * (nn - 2) * (nn - 3);
    return prod / 11;
}
