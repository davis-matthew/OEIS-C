/*
 * A005339  Number of ways in which n identical balls can be distributed among
 *          6 boxes in a row such that each pair of adjacent boxes contains at
 *          least 4 balls.
 * https://oeis.org/A005339
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6), a(12)=35, a(13)=154, a(14)=424, a(15)=930, a(16)=1775,
 *            a(17)=3080, a(18)=4985
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t w[7];
	idx_t i;
	int k;

	w[0] = 35; w[1] = 154; w[2] = 424; w[3] = 930;
	w[4] = 1775; w[5] = 3080; w[6] = 4985;

	if (n < 12) return 0;
	if (n <= 18) return w[n - 12];

	for (i = 19; i <= n; i++) {
		/* w[1..6] hold a(i-6)..a(i-1) after the shift */
		seq_t pos, neg, next;
		for (k = 0; k < 6; k++) w[k] = w[k + 1];
		pos = 6 * w[5] + 20 * w[3] + 6 * w[1];
		neg = 15 * w[4] + 15 * w[2] + w[0];
		next = pos - neg;
		w[6] = next;
	}
	return w[6];
}
