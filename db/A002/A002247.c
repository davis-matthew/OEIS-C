/*
 * A002247  A (6,2)-sequence.
 * https://oeis.org/A002247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   A002247 is a finite (6,2)-sequence with 12 terms
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	static const seq_t vals[12] = {0, 6, 2, 1, 10, 8, 11, 7, 4, 5, 9, 3};
	if (n < 12) {
		return vals[n];
	}
	return 0;
}
