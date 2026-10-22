/*
 * A001845  Centered octahedral numbers (crystal ball sequence for cubic
 *          lattice).
 * https://oeis.org/A001845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n+1)*(2*n^2 + 2*n + 3)/3
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t n_seq = (seq_t)n;
	seq_t factor1 = 2 * n_seq + 1;
	seq_t factor2 = 2 * n_seq * n_seq + 2 * n_seq + 3;
	
	if (factor1 % 3 == 0) {
		return (factor1 / 3) * factor2;
	} else {
		return factor1 * (factor2 / 3);
	}
}
