/*
 * A006360  Antichains (or order ideals) in the poset 2*2*3*n or size of the
 *          distributive lattice J(2*2*3*n).
 * https://oeis.org/A006360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.:
 *            (x+1)*(x^6+36*x^5+279*x^4+594*x^3+279*x^2+36*x+1)/(1-x)^13, i.e.
 *            a(n) = Sum_{k=0..min(n,7)} c(k)*binomial(n-k+12,12) with c =
 *            [1,37,315,873,873,315,37,1]
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom12.h"

seq_t a(idx_t n) {
	static const unsigned int c[8] = {1, 37, 315, 873, 873, 315, 37, 1};
	seq_t result = 0;
	idx_t k;
	for (k = 0; k <= 7 && k <= n; k++) {
		result += (seq_t)c[k] * binom12(n - k);
	}
	return result;
}
