/*
 * A001972  Expansion of 1/((1-x)^2*(1-x^4)) = 1/( (1+x)*(1+x^2)*(1-x)^3 ).
 * https://oeis.org/A001972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)^2/8)
 * Range:     u8 n <= 42, u16 n <= 721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t m = (seq_t)n + 3;
	return m * m / 8;
}
