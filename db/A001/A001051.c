/*
 * A001051  Number of subgroups of order n in orthogonal group O(3).
 * https://oeis.org/A001051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   periodic pattern 1,5,1,7 with period 4, with exceptions at n =
 *            2,4,12,24,48,60,120
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	if (n == 2) return 3;
	if (n == 4) return 5;
	if (n == 12) return 8;
	if (n == 24) return 10;
	if (n == 48) return 8;
	if (n == 60) return 8;
	if (n == 120) return 8;
	
	idx_t r = n % 4;
	if (r == 1) return 1;
	if (r == 2) return 5;
	if (r == 3) return 1;
	return 7;
}
