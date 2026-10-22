/*
 * A002362  Numerators of continued fraction convergents to fifth root of 2.
 * https://oeis.org/A002362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = c(0) = 1, where
 *            c(k) are the continued fraction partial quotients of 2^(1/5),
 *            computed exactly with Lagrange's method: start from P(x) = x^5 -
 *            2, find c = floor(positive root) by galloping Taylor shifts,
 *            then replace P(x) by x^5 * P(c + 1/x) (exact b...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *C[6];
static uint32_t *tmpb;
static size_t L = 0;
static uint64_t *T = NULL;
static size_t nt = 0, tcap = 0;

static void grow_limbs(void) {
	size_t newL = L + 16;
	int i;
	size_t k;
	for (i = 0; i < 6; i++) {
		uint32_t ext = (C[i][L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
		C[i] = (uint32_t *)realloc(C[i], newL * sizeof(uint32_t));
		for (k = L; k < newL; k++) C[i][k] = ext;
	}
	tmpb = (uint32_t *)realloc(tmpb, newL * sizeof(uint32_t));
	L = newL;
}

static void ensure_room(void) {
	int i;
	size_t k;
	for (i = 0; i < 6; i++) {
		uint32_t ext = (C[i][L - 7] >> 31) ? 0xFFFFFFFFu : 0u;
		for (k = L - 6; k < L; k++) {
			if (C[i][k] != ext) { grow_limbs(); return; }
		}
	}
}

static void init_poly(void) {
	int i;
	size_t k;
	L = 16;
	for (i = 0; i < 6; i++) C[i] = (uint32_t *)calloc(L, sizeof(uint32_t));
	tmpb = (uint32_t *)calloc(L, sizeof(uint32_t));
	C[5][0] = 1;
	for (k = 0; k < L; k++) C[0][k] = 0xFFFFFFFFu;
	C[0][0] = 0xFFFFFFFEu; /* -2 */
}

/* dst = dst + s*src, two's complement over L limbs */
static void addmul(uint32_t *dst, const uint32_t *src, uint32_t s) {
	uint64_t carry = 0;
	size_t k;
	for (k = 0; k < L; k++) {
		uint64_t t = (uint64_t)src[k] * s + dst[k] + carry;
		dst[k] = (uint32_t)t;
		carry = t >> 32;
	}
}

/* is the unique positive root of P greater than s? */
static int root_gt(uint32_t s) {
	int j;
	size_t k;
	ensure_room();
	memcpy(tmpb, C[5], L * sizeof(uint32_t));
	for (j = 4; j >= 0; j--) {
		uint64_t carry = 0;
		for (k = 0; k < L; k++) {
			uint64_t t = (uint64_t)tmpb[k] * s + C[j][k] + carry;
			tmpb[k] = (uint32_t)t;
			carry = t >> 32;
		}
	}
	return (tmpb[L - 1] >> 31) == (C[0][L - 1] >> 31);
}

static void shift_by(uint32_t s) {
	int i, j;
	ensure_room();
	for (i = 0; i < 5; i++)
		for (j = 4; j >= i; j--)
			addmul(C[j], C[j + 1], s);
}

static void next_term(void) {
	uint64_t total = 0;
	uint32_t s = 1;
	uint32_t *t;
	while (root_gt(s)) {
		shift_by(s);
		total += s;
		if (s < (1u << 30)) s <<= 1;
	}
	while (s > 1) {
		s >>= 1;
		if (root_gt(s)) {
			shift_by(s);
			total += s;
		}
	}
	t = C[0]; C[0] = C[5]; C[5] = t;
	t = C[1]; C[1] = C[4]; C[4] = t;
	t = C[2]; C[2] = C[3]; C[3] = t;
	if (nt == tcap) {
		tcap = tcap ? tcap * 2 : 256;
		T = (uint64_t *)realloc(T, tcap * sizeof(uint64_t));
	}
	T[nt++] = total;
}

seq_t a(idx_t n) {
	seq_t p_prev, p_cur;
	idx_t i;
	if (L == 0) init_poly();
	while ((idx_t)nt <= n) next_term();
	p_prev = 1;
	p_cur = (seq_t)T[0];
	for (i = 1; i <= n; i++) {
		seq_t p_next = (seq_t)T[i] * p_cur + p_prev;
		p_prev = p_cur;
		p_cur = p_next;
	}
	return p_cur;
}
