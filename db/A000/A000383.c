/*
 * A000383  Hexanacci numbers with a(0) = ... = a(5) = 1.
 * https://oeis.org/A000383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6), a(0)
 *            = a(1) = a(2) = a(3) = a(4) = a(5) = 1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	seq_t w[6];
	idx_t i;
	int j;

	for (j = 0; j < 6; j++) w[j] = 1;
	if (n < 6) return w[0];

	for (i = 6; i <= n; i++) {
		seq_t next = w[0];
		for (j = 1; j < 6; j++) next += w[j];
		for (j = 0; j < 5; j++) w[j] = w[j + 1];
		w[5] = next;
	}
	return w[5];
}
