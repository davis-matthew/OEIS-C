/*
 * A003017  Expansion of 1/(1 - x*exp(x) + x^2*exp(2*x) - x^3*exp(3*x)).
 * https://oeis.org/A003017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = 1/D(x), D(x) = 1 - x*exp(x) + x^2*exp(2*x) -
 *            x^3*exp(3*x). The coefficients of D are [x^m/m!] x^j*exp(j*x) =
 *            m!/(m-j)! * j^(m-j), so a(0) = 1, a(n) = Sum_{k=0..n-1}
 *            binomial(n,k)*a(k)*(e(n-k) - f(n-k)) with e(m) = m +
 *            m*(m-1)*(m-2)*3^(m-3) and f(m) = m*(m-1)*2^(m-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   A_grow, a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/a000272_ipow.h"

static seq_t *A_ = NULL, *R_ = NULL, *DP_ = NULL, *DN_ = NULL;
static idx_t len_ = 0, cap_ = 0;

seq_t a(idx_t n) {
	idx_t m, k;
	if (n >= cap_) {
		idx_t nc = n + 1;
		if (nc < 2 * cap_) nc = 2 * cap_;
		A_ = A_grow(A_, cap_, nc);
		R_ = A_grow(R_, cap_, nc);
		DP_ = A_grow(DP_, cap_, nc);
		DN_ = A_grow(DN_, cap_, nc);
		cap_ = nc;
	}
	if (len_ == 0) {
		A_[0] = 1;
		R_[0] = 1;
		DP_[0] = 0;
		DN_[0] = 0;
		len_ = 1;
	}
	for (m = len_; m <= n; m++) {
		seq_t M = (seq_t)m;
		seq_t pos = 0, neg = 0;
		/* Pascal row: R_ becomes binomial(m, .) */
		R_[m] = 1;
		for (k = m - 1; k >= 1; k--) R_[k] += R_[k - 1];
		/* coefficients of -D at index m, split into positive and negative parts */
		DP_[m] = M;
		if (m >= 3) DP_[m] += M * (M - 1) * (M - 2) * a000272_ipow((seq_t)3, m - 3);
		DN_[m] = 0;
		if (m >= 2) DN_[m] = M * (M - 1) * a000272_ipow((seq_t)2, m - 2);
		for (k = 0; k < m; k++) {
			seq_t t = R_[k] * A_[k];
			idx_t j = m - k;
			pos += t * DP_[j];
			neg += t * DN_[j];
		}
		A_[m] = pos - neg;
	}
	if (n + 1 > len_) len_ = n + 1;
	return A_[n];
}
