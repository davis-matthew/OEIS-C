/*
 * A006671  Edge-distinguishing chromatic number of cycle with n nodes.
 * https://oeis.org/A006671
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   Edge-distinguishing chromatic number of cycle. For r >= 3: if r
 *            is odd and r^2 - 2*r + 1 < 2*n <= r^2 + r, then a(n) = r; if r
 *            is even and r^2 - r < 2*n <= r^2, then a(n) = r
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t two_n = (seq_t)2 * (seq_t)n;

	for (seq_t r = 3; ; r++) {
		seq_t r_sq = r * r;

		if (r % 2 == 1) {
			// r is odd
			seq_t lower = r_sq - (seq_t)2 * r + 1;
			seq_t upper = r_sq + r;
			if (lower < two_n && two_n <= upper) {
				return r;
			}
		} else {
			// r is even
			seq_t lower = r_sq - r;
			seq_t upper = r_sq;
			if (lower < two_n && two_n <= upper) {
				return r;
			}
		}
	}
}
