/*
 * A011876  a(n) = floor(n*(n-1)/23).
 * https://oeis.org/A011876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/23)
 * Range:     u8 n <= 77, u16 n <= 1228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    idx_t nn = n;
    idx_t num = nn * (nn - 1);
    return (seq_t)(num / 23);
}
