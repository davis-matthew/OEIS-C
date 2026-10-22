/*
 * A004127  Number of planar hexagon trees with n hexagons.
 * https://oeis.org/A004127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [z^n] (5*G(z) - 2*G(z)^2 + 9*G(z^2) + 7*z*G(z^2)^3 +
 *            2*z*G(z^3)^2 + 2*z*G(z^6)) / 12, where G(z) = 1 + z*G(z)^5
 *            (A002294), [z^k] G(z)^m = m/(5k+m) * binomial(5k+m, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   bin_s, gpow_coef, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/bin_s.h"
#include "../utils/gpow_coef.h"

typedef unsigned __int128 u128;

static seq_t a_big(idx_t n) {
	seq_t s = 5 * gpow_coef(1, n);
	if (n % 2 == 0) s += 9 * gpow_coef(1, n / 2);
	if ((n - 1) % 2 == 0) s += 7 * gpow_coef(3, (n - 1) / 2);
	if ((n - 1) % 3 == 0) s += 2 * gpow_coef(2, (n - 1) / 3);
	if ((n - 1) % 6 == 0) s += 2 * gpow_coef(1, (n - 1) / 6);
	s -= 2 * gpow_coef(2, n);
	return s / 12;
}

/* ---------- fixed width path: series mod 2^128 and mod 15 ---------- */
static u128 *fG = NULL, *f2 = NULL, *f3 = NULL, *f4 = NULL;
static unsigned *mG = NULL, *m2 = NULL, *m3 = NULL, *m4 = NULL;
static idx_t flen = 0;

static void extend(idx_t N) {
	if (flen > N) return;
	size_t sz = (size_t)(N + 1);
	fG = (u128 *)realloc(fG, sz * sizeof(u128));
	f2 = (u128 *)realloc(f2, sz * sizeof(u128));
	f3 = (u128 *)realloc(f3, sz * sizeof(u128));
	f4 = (u128 *)realloc(f4, sz * sizeof(u128));
	mG = (unsigned *)realloc(mG, sz * sizeof(unsigned));
	m2 = (unsigned *)realloc(m2, sz * sizeof(unsigned));
	m3 = (unsigned *)realloc(m3, sz * sizeof(unsigned));
	m4 = (unsigned *)realloc(m4, sz * sizeof(unsigned));
	for (idx_t i = flen; i <= N; i++) {
		if (i == 0) {
			fG[0] = f2[0] = f3[0] = f4[0] = 1;
			mG[0] = m2[0] = m3[0] = m4[0] = 1;
			continue;
		}
		u128 x = 0; unsigned long long y = 0;
		for (idx_t t = 0; t < i; t++) {
			x += f4[t] * fG[i - 1 - t];
			y += m4[t] * mG[i - 1 - t];
		}
		fG[i] = x; mG[i] = (unsigned)(y % 15);
		u128 x2 = 0, x3 = 0, x4 = 0;
		unsigned long long y2 = 0, y3 = 0, y4 = 0;
		for (idx_t t = 0; t <= i; t++) {
			x2 += fG[t] * fG[i - t];
			y2 += mG[t] * mG[i - t];
		}
		f2[i] = x2; m2[i] = (unsigned)(y2 % 15);
		for (idx_t t = 0; t <= i; t++) {
			x3 += f2[t] * fG[i - t];
			y3 += m2[t] * mG[i - t];
			x4 += f2[t] * f2[i - t];
			y4 += m2[t] * m2[i - t];
		}
		f3[i] = x3; m3[i] = (unsigned)(y3 % 15);
		f4[i] = x4; m4[i] = (unsigned)(y4 % 15);
	}
	flen = N + 1;
}

static seq_t a_fixed(idx_t n) {
	extend(n);
	u128 r = 5 * fG[n] - 2 * f2[n];
	unsigned s = (5 * mG[n] + 30 - 2 * m2[n]) % 15;
	if (n % 2 == 0) { r += 9 * fG[n / 2]; s += 9 * mG[n / 2]; }
	if ((n - 1) % 2 == 0) { r += 7 * f3[(n - 1) / 2]; s += 7 * m3[(n - 1) / 2]; }
	if ((n - 1) % 3 == 0) { r += 2 * f2[(n - 1) / 3]; s += 2 * m2[(n - 1) / 3]; }
	if ((n - 1) % 6 == 0) { r += 2 * fG[(n - 1) / 6]; s += 2 * mG[(n - 1) / 6]; }
	s %= 15;
	/* true S = r + k*2^128 with 0 <= k < 15; 2^128 == 1 (mod 15) */
	unsigned k = (unsigned)((s + 15 - (unsigned)(r % 15)) % 15);
	/* 2^128 = 12*Q + 4 */
	u128 Q = (~(u128)0) / 12;
	u128 res = (u128)k * Q + r / 12 + ((u128)(r % 12) + 4 * (u128)k) / 12;
	return u128_to_seq(res);
}

seq_t a(idx_t n) {
	if (n < 1) return 0;
	if (seq_is_fixed_width()) return a_fixed(n);
	return a_big(n);
}
