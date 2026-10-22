/*
 * A003165  a(n) = floor(n/2) + 1 - d(n), where d(n) is the number of divisors
 *          of n.
 * https://oeis.org/A003165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + 1 - d(n), where d(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 518, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	/* Count the number of divisors of n */
	idx_t divisor_count = 0;

	for (idx_t i = 1; i * i <= n; i++) {
		if (n % i == 0) {
			divisor_count++;
			if (i * i != n) {
				divisor_count++;
			}
		}
	}

	/* Compute a(n) = floor(n/2) + 1 - d(n) */
	seq_t sn = (seq_t)n;
	seq_t result = sn / 2 + 1 - (seq_t)divisor_count;
	return result;
}
