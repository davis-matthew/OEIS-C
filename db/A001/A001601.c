/*
 * A001601  a(n) = 2*a(n-1)^2 - 1, if n>1. a(0)=1, a(1)=3.
 * https://oeis.org/A001601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = 2*a(n-1)^2 - 1, a(0) = 1, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 0) return (seq_t)1;
	if (n == 1) return (seq_t)3;
	
	seq_t prev = (seq_t)3;
	for (idx_t i = 2; i <= n; i++) {
		seq_t next = 2 * prev * prev - 1;
		prev = next;
	}
	return prev;
}
