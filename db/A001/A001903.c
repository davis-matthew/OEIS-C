/*
 * A001903  Final digit of 7^n.
 * https://oeis.org/A001903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 10 (period 4: [1, 7, 9, 3])
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
	static const seq_t cycle[] = {1, 7, 9, 3};
	return cycle[n % 4];
}
