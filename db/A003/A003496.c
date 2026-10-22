/*
 * A003496  High temperature series for spherical model internal energy on
 *          3-dimensional simple cubic lattice.
 * https://oeis.org/A003496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = [t^n] (G(t) + 2*t*G'(t)) / G(t)^(2n), where G(t) =
 *            Sum_{k>=0} A002896(k) t^k and A002896(k) = C(2k,k) *
 *            Sum_{i=0..k} C(k,i)^2 * C(2(k-i),k-i) (closed walks on the
 *            simple cubic lattice). This is Lagrange inversion of K^2 =
 *            t*G(t)^2: the generating function Sum a(n) K^(2n) equals
 *            G(t(K^2))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   a3496_mul, a3496_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a3496_mul.h"
#include "../utils/a3496_zero.h"

seq_t a(idx_t n)
{
	idx_t L = n + 1;
	idx_t W = 2 * n + 1;
	seq_t *cb = a3496_zero(L);
	seq_t *row = a3496_zero(W);
	seq_t *c = a3496_zero(L);
	seq_t *R = a3496_zero(L);
	seq_t *B = a3496_zero(L);
	seq_t *T = a3496_zero(L);
	seq_t *qp = a3496_zero(L);
	seq_t *qm = a3496_zero(L);
	idx_t r, j, i, k;

	/* central binomials C(2m,m), m = 0..n */
	row[0] = (seq_t)1;
	cb[0] = (seq_t)1;
	for (r = 1; r < W; r++) {
		for (j = r; j >= 1; j--) row[j] += row[j - 1];
		if (r % 2 == 0) cb[r / 2] = row[r / 2];
	}

	/* A002896(k), k = 0..n */
	for (j = 0; j < W; j++) row[j] = (seq_t)0;
	row[0] = (seq_t)1;
	for (r = 0; r <= n; r++) {
		if (r > 0)
			for (j = r; j >= 1; j--) row[j] += row[j - 1];
		seq_t s = (seq_t)0;
		for (i = 0; i <= r; i++) s += row[i] * row[i] * cb[r - i];
		c[r] = cb[r] * s;
	}

	/* R = G^(2n) truncated */
	R[0] = (seq_t)1;
	for (k = 0; k < L; k++) B[k] = c[k];
	{
		idx_t e = 2 * n;
		while (e) {
			if (e & 1) {
				a3496_mul(R, B, T, L);
				for (k = 0; k < L; k++) R[k] = T[k];
			}
			e >>= 1;
			if (e) {
				a3496_mul(B, B, T, L);
				for (k = 0; k < L; k++) B[k] = T[k];
			}
		}
	}

	/* Q = (G + 2tG') / R, signed as qp - qm */
	for (k = 0; k <= n; k++) {
		seq_t pos = c[k] * ((seq_t)k * 2 + 1);
		seq_t neg = (seq_t)0;
		for (j = 1; j <= k; j++) {
			pos += R[j] * qm[k - j];
			neg += R[j] * qp[k - j];
		}
		if (pos >= neg) {
			qp[k] = pos - neg;
			qm[k] = (seq_t)0;
		} else {
			qm[k] = neg - pos;
			qp[k] = (seq_t)0;
		}
	}

	seq_t result = qp[n] - qm[n];
	free(cb); free(row); free(c); free(R); free(B); free(T); free(qp); free(qm);
	return result;
}
