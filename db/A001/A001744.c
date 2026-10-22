/*
 * A001744  Numbers n such that every digit contains a loop (version 2).
 * https://oeis.org/A001744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   If n-1 in base 5 is d_0 d_1 ... d_m, then a(n) = sum_{j=0..m}
 *            c(d_j)*10^j where c(0)=0, c(1)=4, c(2)=6, c(3)=8, c(4)=9
 * Range:     u8 n <= 25, u16 n <= 1500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	static const int digit_map[] = {0, 4, 6, 8, 9};
	
	idx_t k = n - 1;
	seq_t result = 0;
	seq_t power_of_10 = 1;
	
	while (k > 0) {
		int base5_digit = k % 5;
		result += (seq_t)digit_map[base5_digit] * power_of_10;
		power_of_10 *= 10;
		k /= 5;
	}
	
	return result;
}
