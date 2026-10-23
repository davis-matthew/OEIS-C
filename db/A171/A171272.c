/*
 * A171272  a(n) = 1 + 4*n*(1 + 2*n^2)/3.
 * https://oeis.org/A171272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 4*n*(1 + 2*n^2)/3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t inner = 1 + 2 * n2;
    seq_t prod = (4 * nn) * inner;
    return prod / 3 + 1;
}
