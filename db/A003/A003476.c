/*
 * A003476  a(n) = a(n-1) + 2*a(n-3).
 * https://oeis.org/A003476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3), with a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 1) return 1;
	if (n == 2) return 2;
	if (n == 3) return 3;
	
	seq_t a_n_minus_3 = 1;
	seq_t a_n_minus_2 = 2;
	seq_t a_n_minus_1 = 3;
	
	for (idx_t i = 4; i <= n; i++) {
		seq_t a_i = a_n_minus_1 + 2 * a_n_minus_3;
		a_n_minus_3 = a_n_minus_2;
		a_n_minus_2 = a_n_minus_1;
		a_n_minus_1 = a_i;
	}
	
	return a_n_minus_1;
}
