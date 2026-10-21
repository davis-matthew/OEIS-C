/*
 * A226725  Denominator of the median of {1, 1/2, 1/3, ..., 1/n}.
 * https://oeis.org/A226725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)/2 if n is odd; a(n) = n*(n/2+1) if n is even
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) != 0) {
        return (nn + 1) / 2;
    } else {
        seq_t half = nn / 2;
        return nn * (half + 1);
    }
}
