/*
 * A001846  Centered 4-dimensional orthoplex numbers (crystal ball sequence
 *          for 4-dimensional cubic lattice).
 * https://oeis.org/A001846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 + 4*n^3 + 10*n^2 + 8*n + 3) / 3
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	seq_t n_seq = (seq_t)n;
	seq_t n2 = n_seq * n_seq;
	seq_t n3 = n2 * n_seq;
	seq_t n4 = n3 * n_seq;
	
	seq_t numerator = 2*n4 + 4*n3 + 10*n2 + 8*n_seq + 3;
	return numerator / 3;
}
