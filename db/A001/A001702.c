/*
 * A001702  Generalized Stirling numbers, [n+3,n]_2.
 * https://oeis.org/A001702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/48)*(n-1)*n*(n+1)*(n+4)*(n^2+7n+14), n > 1; a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 1) return (seq_t)1;
	
	seq_t nn = (seq_t)n;
	seq_t prod = (nn - 1) * nn * (nn + 1);
	prod = prod / 6;
	
	seq_t poly = nn * nn + 7 * nn + 14;
	prod = prod * (nn + 4) * poly / 8;
	
	return prod;
}
