/*
 * A002597  Number of partitions into one kind of 1's, two kinds of 2's, and
 *          three kinds of 3's.
 * https://oeis.org/A002597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/((1-x)*(1-x^2)^2*(1-x^3)^3); a(n) is computed as the
 *            coefficient of x^n by dividing the series 1 by each factor in
 *            turn, using c(i) = c(i) + c(i-k) for each factor 1/(1-x^k)
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 554, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	static const int ks[6] = {1, 2, 2, 3, 3, 3};
	seq_t buf[6][3];
	seq_t v = 0;
	int s, j;
	idx_t i;

	for (s = 0; s < 6; s++)
		for (j = 0; j < 3; j++)
			buf[s][j] = 0;

	for (i = 0; i <= n; i++) {
		v = (i == 0) ? 1 : 0;
		for (s = 0; s < 6; s++) {
			idx_t k = (idx_t)ks[s];
			if (i >= k)
				v += buf[s][(int)((i - k) % 3)];
			buf[s][(int)(i % 3)] = v;
		}
	}
	return v;
}
