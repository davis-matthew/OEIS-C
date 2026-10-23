/*
 * A011886  a(n) = floor(n*(n-1)*(n-2)/4).
 * https://oeis.org/A011886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/4)
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn - 1) * (nn - 2) / 4;
    return term;
}
