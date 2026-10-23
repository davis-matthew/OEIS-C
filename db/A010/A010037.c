/*
 * A010037  Numbers n such that gcd(n^5 + 5, (n+1)^5 + 5) > 1.
 * https://oeis.org/A010037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 533360 + (n-1)*1968751
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)533360 + (N - 1) * (seq_t)1968751;
}
