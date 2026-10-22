/*
 * A002414  Octagonal pyramidal numbers: a(n) = n*(n+1)*(2*n-1)/2.
 * https://oeis.org/A002414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(2*n-1)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t n_seq = (seq_t)n;
	seq_t np1 = n_seq + 1;
	seq_t two_n_minus_1 = 2 * n_seq - 1;
	
	if (n % 2 == 0) {
		return (n_seq / 2) * np1 * two_n_minus_1;
	} else {
		return n_seq * (np1 / 2) * two_n_minus_1;
	}
}
