/*
 * A002626  Expansion of 1/((1-x)^3 (1-x^2)^2 (1-x^3) (1-x^4)).
 * https://oeis.org/A002626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(n+13)*(135*(-1)^n + 2*n^4 + 56*n^3 + 570*n^2
 *            + 2492*n + 4175)/69120 + 1/2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 29, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t n_seq = (seq_t)n;
	
	// Compute powers of n
	seq_t n2 = n_seq * n_seq;
	seq_t n3 = n2 * n_seq;
	seq_t n4 = n3 * n_seq;
	
	// Compute P = 135*(-1)^n + 2*n^4 + 56*n^3 + 570*n^2 + 2492*n + 4175
	seq_t poly = 2 * n4 + 56 * n3 + 570 * n2 + 2492 * n_seq + 4175;
	
	seq_t P;
	if (n & 1) {
		// n is odd, (-1)^n = -1
		P = poly - 135;
	} else {
		// n is even, (-1)^n = 1
		P = poly + 135;
	}
	
	// Compute M = (n+1)*(n+13)*P
	seq_t M = (n_seq + 1) * (n_seq + 13) * P;
	
	// a(n) = (M + 34560) / 69120 (rounding to nearest integer)
	seq_t result = (M + 34560) / 69120;
	
	return result;
}
