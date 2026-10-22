/*
 * A003498  High temperature series for internal energy for spherical model on
 *          f.c.c. lattice.
 * https://oeis.org/A003498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   spherical-model saddle point on the f.c.c. lattice. Let r(k) =
 *            A002899(k) be the number of closed k-step f.c.c. walks: r(k) =
 *            Sum_{a+b+c=k, a,b,c same parity} k!/(a!b!c!) *
 *            C(a+b,(a+b)/2)*C(b+c,(b+c)/2)*C(a+c,(a+c)/2). Let F(x) = Sum_k
 *            r(k) x^k and invert t = x*F(x). Then a(n) = [t^(n+1)] t/x(t)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   mulv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mulv.h"

#define MAXM 200

static int big_mode = -1;
static int done_M = 0;

static seq_t pas[2 * MAXM + 1][2 * MAXM + 1];
static seq_t rr[MAXM + 1];
static seq_t wm[MAXM + 1];
static unsigned char ws[MAXM + 1];
static seq_t pm[MAXM + 2][MAXM + 1];
static unsigned char ps[MAXM + 2][MAXM + 1];
static seq_t ym[MAXM + 1];
static unsigned char ys[MAXM + 1];

static void detect_big(void) {
	if (big_mode < 0) {
		seq_t x = 1;
		int i;
		for (i = 0; i < 200; i++) x <<= 1;
		big_mode = (x != 0) ? 1 : 0;
	}
}

/* (*am,*as) += (vs ? -v : v); sign-magnitude in big mode, wrapping otherwise */
static void sacc(seq_t *am, unsigned char *as, seq_t v, int vs) {
	if (!big_mode) {
		if (vs) *am = (seq_t)(*am - v);
		else *am = (seq_t)(*am + v);
		return;
	}
	if ((int)*as == vs) { *am += v; }
	else if (*am >= v) { *am -= v; }
	else { *am = (seq_t)(v - *am); *as = (unsigned char)vs; }
	if (*am == 0) *as = 0;
}

static void compute(idx_t M) {
	idx_t i, j, k, a, b, m, p;
	idx_t R = 2 * M;
	for (i = 0; i <= R; i++) {
		pas[i][0] = 1;
		pas[i][i] = 1;
		for (j = 1; j < i; j++) pas[i][j] = (seq_t)(pas[i - 1][j - 1] + pas[i - 1][j]);
	}
	for (k = 0; k <= M; k++) {
		seq_t s = 0;
		for (a = 0; a <= k; a++) {
			for (b = 0; a + b <= k; b++) {
				idx_t c = k - a - b;
				seq_t t;
				if (((a + b) & 1) || ((b + c) & 1) || ((a + c) & 1)) continue;
				t = mulv(pas[k][a], pas[k - a][b]);
				t = mulv(t, pas[a + b][(a + b) / 2]);
				t = mulv(t, pas[b + c][(b + c) / 2]);
				t = mulv(t, pas[a + c][(a + c) / 2]);
				s = (seq_t)(s + t);
			}
		}
		rr[k] = s;
	}
	wm[0] = 1; ws[0] = 0;
	for (p = 0; p <= M + 1; p++) { pm[p][0] = 1; ps[p][0] = 0; }
	for (m = 1; m <= M; m++) { pm[0][m] = 0; ps[0][m] = 0; }
	for (m = 1; m <= M; m++) {
		seq_t acc = 0;
		unsigned char as = 0;
		for (k = 1; k <= m; k++) {
			seq_t t;
			if (rr[k] == 0 || pm[k + 1][m - k] == 0) continue;
			t = mulv(rr[k], pm[k + 1][m - k]);
			sacc(&acc, &as, t, !ps[k + 1][m - k]);
		}
		wm[m] = acc; ws[m] = as;
		for (p = 1; p <= M - m + 1; p++) {
			seq_t s = 0;
			unsigned char ss = 0;
			for (j = 0; j <= m; j++) {
				seq_t t;
				if (wm[j] == 0 || pm[p - 1][m - j] == 0) continue;
				t = mulv(wm[j], pm[p - 1][m - j]);
				sacc(&s, &ss, t, ws[j] ^ ps[p - 1][m - j]);
			}
			pm[p][m] = s; ps[p][m] = ss;
		}
	}
	ym[0] = 1; ys[0] = 0;
	for (m = 1; m <= M; m++) {
		seq_t s = 0;
		unsigned char ss = 0;
		for (j = 1; j <= m; j++) {
			seq_t t;
			if (wm[j] == 0 || ym[m - j] == 0) continue;
			t = mulv(wm[j], ym[m - j]);
			sacc(&s, &ss, t, !(ws[j] ^ ys[m - j]));
		}
		ym[m] = s; ys[m] = ss;
	}
	done_M = (int)M;
}

seq_t a(idx_t n) {
	idx_t need;
	if (n == 0) return (seq_t)1;
	detect_big();
	need = n + 1;
	if (need > MAXM) return (seq_t)0;
	if ((idx_t)done_M < need) {
		idx_t M = need;
		idx_t dbl = (idx_t)done_M * 2;
		if (dbl > M) M = dbl;
		if (M < 24) M = 24;
		if (M > MAXM) M = MAXM;
		compute(M);
	}
	return ym[need];
}
