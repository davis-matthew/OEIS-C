/*
 * A001634  a(n) = a(n-2) + a(n-3) + a(n-4), with initial values a(0) = 0,
 *          a(1) = 2, a(2) = 3, a(3) = 6.
 * https://oeis.org/A001634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4), with a(0) = 0, a(1) = 2, a(2) =
 *            3, a(3) = 6
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 0) return (seq_t)0;
	if (n == 1) return (seq_t)2;
	if (n == 2) return (seq_t)3;
	if (n == 3) return (seq_t)6;

	seq_t a_nm4 = (seq_t)0;
	seq_t a_nm3 = (seq_t)2;
	seq_t a_nm2 = (seq_t)3;
	seq_t a_n = (seq_t)6;

	for (idx_t i = 4; i <= n; i++) {
		seq_t next = a_nm2 + a_nm3 + a_nm4;
		a_nm4 = a_nm3;
		a_nm3 = a_nm2;
		a_nm2 = a_n;
		a_n = next;
	}

	return a_n;
}
