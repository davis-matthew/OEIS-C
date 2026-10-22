/*
 * A002699  a(n) = n*2^(2*n-1).
 * https://oeis.org/A002699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n * 2^(2*n - 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 0) return 0;
	return ((seq_t)n) << (2 * n - 1);
}
