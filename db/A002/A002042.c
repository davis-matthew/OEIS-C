/*
 * A002042  a(n) = 7*4^n.
 * https://oeis.org/A002042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1), a(0) = 7 (i.e. a(n) = 7*4^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t r = (seq_t)7;
	idx_t i;
	for (i = 0; i < n; i++)
		r *= 4;
	return r;
}
