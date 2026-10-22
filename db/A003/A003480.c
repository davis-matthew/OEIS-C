/*
 * A003480  a(0) = 1, a(1) = 2, a(2) = 7, for n > 2, a(n) = 4*a(n-1) -
 *          2*a(n-2).
 * https://oeis.org/A003480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = 2, a(2) = 7
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 0) return (seq_t)1;
	if (n == 1) return (seq_t)2;
	if (n == 2) return (seq_t)7;

	seq_t prev2 = (seq_t)2;
	seq_t prev1 = (seq_t)7;
	seq_t curr;

	for (idx_t i = 3; i <= n; i++) {
		curr = (seq_t)4 * prev1 - (seq_t)2 * prev2;
		prev2 = prev1;
		prev1 = curr;
	}

	return curr;
}
