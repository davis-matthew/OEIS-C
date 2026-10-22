/*
 * A007311  Reversion of o.g.f. for Bell numbers (A000110) omitting a(0)=1.
 * https://oeis.org/A007311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   A(x) = x - Sum_{k>=2} Bell(k)*A(x)^k, i.e. A is the series
 *            reversion of x*B(x) with B(x) = Sum_{k>=0} Bell(k+1) x^k. By
 *            Lagrange inversion, a(n) = (1/n) [x^(n-1)] H(x)^n with H = 1/B.
 *            The power H^n is built with the J.C.P. Miller recurrence.
 *            Fixed-width builds instead compute A^k exactly mod 2^128
 * Range:     i8 n <= 9, i16 n <= 12, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* ---------- fixed-width path: exact ring arithmetic mod 2^128 ---------- */
#define FIXN 160
typedef unsigned __int128 u128_t;
static u128_t fx_a[FIXN + 1];
static u128_t fx_pw[FIXN + 1][FIXN + 1];
static bool fx_done = false;

static void fx_init(void) {
	u128_t bell[FIXN + 1];
	u128_t r0[FIXN + 2], r1[FIXN + 2];
	idx_t i, j, k, m;
	r0[0] = 1;
	bell[0] = 1;
	for (i = 1; i <= FIXN; i++) {
		r1[0] = r0[i - 1];
		for (j = 1; j <= i; j++) r1[j] = r1[j - 1] + r0[j - 1];
		bell[i] = r1[0];
		for (j = 0; j <= i; j++) r0[j] = r1[j];
	}
	for (m = 1; m <= FIXN; m++) {
		u128_t tot = 0;
		for (k = 2; k <= m; k++) {
			u128_t s = 0;
			for (j = 1; j + k - 1 <= m; j++) s += fx_a[j] * fx_pw[k - 1][m - j];
			fx_pw[k][m] = s;
			tot += bell[k] * s;
		}
		fx_a[m] = (m == 1) ? (u128_t)1 : (u128_t)0 - tot;
		fx_pw[1][m] = fx_a[m];
	}
	fx_done = true;
}

/* ---------- arbitrary-precision path: Lagrange inversion ---------- */
static seq_t *big_H = NULL;
static idx_t big_Hlen = 0;

static void big_ensure_H(idx_t L) {
	idx_t i, j, m;
	seq_t *bell, *r0, *r1;
	if (L <= big_Hlen) return;
	if (L < 2 * big_Hlen) L = 2 * big_Hlen;
	bell = alloc_zero(L + 2);
	r0 = alloc_zero(L + 2);
	r1 = alloc_zero(L + 2);
	r0[0] = 1;
	bell[0] = 1;
	for (i = 1; i <= L; i++) {
		r1[0] = r0[i - 1];
		for (j = 1; j <= i; j++) r1[j] = r1[j - 1] + r0[j - 1];
		bell[i] = r1[0];
		for (j = 0; j <= i; j++) r0[j] = r1[j];
	}
	release(r0, L + 2);
	release(r1, L + 2);
	if (big_H) release(big_H, big_Hlen);
	big_H = alloc_zero(L);
	/* H = 1 / (Sum_{k>=0} Bell(k+1) x^k) */
	big_H[0] = 1;
	for (m = 1; m < L; m++) {
		seq_t s = 0;
		for (j = 1; j <= m; j++) s += bell[j + 1] * big_H[m - j];
		big_H[m] = -s;
	}
	release(bell, L + 2);
	big_Hlen = L;
}

static seq_t *big_memo = NULL;
static bool *big_known = NULL;
static idx_t big_memolen = 0;

static seq_t big_a(idx_t n) {
	idx_t j, k;
	seq_t *P;
	seq_t r;
	if (n < big_memolen && big_known[n]) return big_memo[n];
	big_ensure_H(n);
	P = alloc_zero(n);
	P[0] = 1;
	for (k = 1; k < n; k++) {
		seq_t s = 0;
		for (j = 1; j <= k; j++)
			s += (seq_t)((n + 1) * j - k) * big_H[j] * P[k - j];
		P[k] = s / (seq_t)k;
	}
	r = P[n - 1] / (seq_t)n;
	release(P, n);
	if (n >= big_memolen) {
		idx_t nl = n + 64, t;
		seq_t *nm = alloc_zero(nl);
		bool *nk = (bool *)malloc(nl * sizeof(bool));
		for (t = 0; t < nl; t++) nk[t] = false;
		for (t = 0; t < big_memolen; t++) {
			nk[t] = big_known[t];
			nm[t] = big_memo[t];
		}
		if (big_memo) release(big_memo, big_memolen);
		if (big_known) free(big_known);
		big_memo = nm;
		big_known = nk;
		big_memolen = nl;
	}
	big_memo[n] = r;
	big_known[n] = true;
	return r;
}

seq_t a(idx_t n) {
	if (n < 1) return 0;
	if (seq_is_fixed_width()) {
		if (n > FIXN) return 0; /* |a(n)| far exceeds 128 bits here */
		if (!fx_done) fx_init();
		return from_i128((__int128)fx_a[n]);
	}
	return big_a(n);
}
