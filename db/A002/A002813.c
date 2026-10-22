/*
 * A002813  a(0) = 4; for n > 0, a(n) = a(n-1)^3 - 3*a(n-1)^2 + 3.
 * https://oeis.org/A002813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 4; for n > 0, a(n) = a(n-1)^3 - 3*a(n-1)^2 + 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 0) return (seq_t)4;
	
	seq_t val = (seq_t)4;
	for (idx_t i = 1; i <= n; i++) {
		seq_t prev = val;
		seq_t prev_sq = prev * prev;
		val = prev_sq * prev - (seq_t)3 * prev_sq + (seq_t)3;
	}
	return val;
}
