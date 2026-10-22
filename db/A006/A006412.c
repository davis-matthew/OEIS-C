/*
 * A006412  Number of nonseparable tree-rooted planar maps with n + 3 edges
 *          and 4 vertices.
 * https://oeis.org/A006412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(n+4,5) + 51*binomial(n+4,6) +
 *            163*binomial(n+4,7) + 194*binomial(n+4,8) + 78*binomial(n+4,9)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 350, u128 n <= 1000,
 *            big any n
 * Helpers:   a006412_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006412_binom.h"

seq_t a(idx_t n) {
	idx_t m = n + 4;
	seq_t s = 0;
	s += (seq_t)4 * a006412_binom(m, 5);
	s += (seq_t)51 * a006412_binom(m, 6);
	s += (seq_t)163 * a006412_binom(m, 7);
	s += (seq_t)194 * a006412_binom(m, 8);
	s += (seq_t)78 * a006412_binom(m, 9);
	return s;
}
