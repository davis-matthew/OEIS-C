/*
 * A003246  Discriminants of real quadratic norm-Euclidean fields (a finite
 *          sequence).
 * https://oeis.org/A003246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, nice
 * Formula:   discriminants of real quadratic norm-Euclidean fields (complete
 *            list: 16 terms total)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	static const seq_t values[] = {5, 8, 12, 13, 17, 21, 24, 28, 29, 33, 37, 41, 44, 57, 73, 76};
	
	if (n < 1 || n > 16)
		return 0;
	
	return values[n - 1];
}
