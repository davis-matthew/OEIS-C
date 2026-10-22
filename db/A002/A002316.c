/*
 * A002316  Related to Bernoulli numbers.
 * https://oeis.org/A002316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) - 6*a(n-3) - a(n-4), a(0) = 1, a(1)
 *            = 5, a(2) = 26, a(3) = 97
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 32, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	if (n == 0) return 1;
	if (n == 1) return 5;
	if (n == 2) return 26;
	if (n == 3) return 97;

	seq_t a0 = 1, a1 = 5, a2 = 26, a3 = 97;

	for (idx_t i = 4; i <= n; i++) {
		seq_t next = 6 * a3 - 11 * a2 - 6 * a1 - a0;
		a0 = a1;
		a1 = a2;
		a2 = a3;
		a3 = next;
	}

	return a3;
}
