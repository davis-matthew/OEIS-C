/*
 * A008748  Expansion of (1 + x^5) / ((1-x) * (1-x^2) * (1-x^3)) in powers of
 *          x.
 * https://oeis.org/A008748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 + floor(n*(n+1)/6)
 * Range:     u8 n <= 38, u16 n <= 626, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn + 1) / 6 + 1;
}
