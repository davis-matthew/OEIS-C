/*
 * A002798  a(n) = a(n-1) + a(n-2) - a(n-3), starting with 18, 45, and 69.
 * https://oeis.org/A002798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (51*n - 12)/2 if n even, (51*n - 15)/2 if n odd
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t n_seq = (seq_t)n;
	return (51 * n_seq - ((n & 1) ? 15 : 12)) / 2;
}
