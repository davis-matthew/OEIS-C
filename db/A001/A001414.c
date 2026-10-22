/*
 * A001414  Integer log of n: sum of primes dividing n (with repetition). Also
 *          called sopfr(n).
 * https://oeis.org/A001414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum p_j * k_j where n = Product p_j^k_j
 * Range:     u8 n <= 256, u16 n <= 9837, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 1) return 0;
	
	seq_t result = 0;
	idx_t temp = n;
	
	/* Handle factor of 2 */
	idx_t count = 0;
	while (temp % 2 == 0) {
		temp /= 2;
		count++;
	}
	result += (seq_t)2 * (seq_t)count;
	
	/* Handle odd factors */
	for (idx_t p = 3; temp / p >= p; p += 2) {
		count = 0;
		while (temp % p == 0) {
			temp /= p;
			count++;
		}
		result += (seq_t)p * (seq_t)count;
	}
	
	/* Remaining factor if temp > 1 */
	if (temp > 1) {
		result += (seq_t)temp;
	}
	
	return result;
}
