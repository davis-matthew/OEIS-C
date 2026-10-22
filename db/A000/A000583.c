/*
 * A000583  Fourth powers: a(n) = n^4.
 * https://oeis.org/A000583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice, mult
 * Formula:   a(n) = n^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t n_s = (seq_t)n;
	seq_t n2 = n_s * n_s;
	return n2 * n2;
}
