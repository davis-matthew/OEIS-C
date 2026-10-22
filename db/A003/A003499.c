/*
 * A003499  a(n) = 6*a(n-1) - a(n-2), with a(0) = 2, a(1) = 6.
 * https://oeis.org/A003499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0) = 2, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 0) return (seq_t)2;
	if (n == 1) return (seq_t)6;
	
	seq_t prev = (seq_t)2;
	seq_t curr = (seq_t)6;
	
	for (idx_t i = 2; i <= n; i++) {
		seq_t next = (seq_t)6 * curr - prev;
		prev = curr;
		curr = next;
	}
	
	return curr;
}
