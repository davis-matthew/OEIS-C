/*
 * A000562  Number of discordant permutations.
 * https://oeis.org/A000562
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n^4 - 270*n^3 + 921*n^2 - 1078*n) / 8 for n >= 5;
 *            a(4) = 9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 4) return 9;
	
	seq_t n_seq = (seq_t)n;
	seq_t n2 = n_seq * n_seq;
	seq_t n3 = n2 * n_seq;
	seq_t n4 = n3 * n_seq;
	
	seq_t numerator = 27 * n4 - 270 * n3 + 921 * n2 - 1078 * n_seq;
	return numerator / 8;
}
