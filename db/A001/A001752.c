/*
 * A001752  Expansion of 1/((1+x)*(1-x)^5).
 * https://oeis.org/A001752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+3)^2 - 1)*((n+3)^2 - 3)/48)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 496, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t m = (seq_t)n + 3;
	seq_t m2 = m * m;
	seq_t a1 = m2 - 1;
	seq_t a2 = m2 - 3;
	return (a1 * a2) / 48;
}
