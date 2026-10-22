/*
 * A002666  Continued cotangent for square root of 2.
 * https://oeis.org/A002666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lehmer continued cotangent of sqrt(2): x(0) = sqrt(2), a(n) =
 *            floor(x(n)), x(n+1) = (a(n)*x(n) + 1)/(x(n) - a(n)). x(n) is
 *            kept exactly as (P + C*sqrt(2))/R with big integers
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; int neg; } a2666_bn;

static a2666_bn a2666_alloc(size_t n)
{
	a2666_bn r;
	r.d = (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
	r.n = n;
	r.neg = 0;
	return r;
}

static void a2666_trim(a2666_bn *a)
{
	while (a->n && a->d[a->n - 1] == 0) a->n--;
	if (a->n == 0) a->neg = 0;
}

static void a2666_free(a2666_bn *a)
{
	free(a->d);
	a->d = NULL;
	a->n = 0;
	a->neg = 0;
}

static void a2666_negate(a2666_bn *a)
{
	if (a->n) a->neg = !a->neg;
}

static a2666_bn a2666_u64(uint64_t v)
{
	a2666_bn r = a2666_alloc(2);
	r.d[0] = (uint32_t)v;
	r.d[1] = (uint32_t)(v >> 32);
	a2666_trim(&r);
	return r;
}

static a2666_bn a2666_copy(const a2666_bn *a)
{
	a2666_bn r = a2666_alloc(a->n);
	if (a->n) memcpy(r.d, a->d, a->n * sizeof(uint32_t));
	r.neg = a->neg;
	return r;
}

static int a2666_cmpabs(const a2666_bn *a, const a2666_bn *b)
{
	size_t i;
	if (a->n != b->n) return a->n < b->n ? -1 : 1;
	for (i = a->n; i-- > 0;)
		if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
	return 0;
}

static a2666_bn a2666_addabs(const a2666_bn *a, const a2666_bn *b)
{
	size_t L = a->n > b->n ? a->n : b->n, i;
	a2666_bn r = a2666_alloc(L + 1);
	uint64_t c = 0;
	for (i = 0; i < L; i++) {
		c += (uint64_t)(i < a->n ? a->d[i] : 0) + (uint64_t)(i < b->n ? b->d[i] : 0);
		r.d[i] = (uint32_t)c;
		c >>= 32;
	}
	r.d[L] = (uint32_t)c;
	a2666_trim(&r);
	return r;
}

/* requires |a| >= |b| */
static a2666_bn a2666_subabs(const a2666_bn *a, const a2666_bn *b)
{
	size_t i;
	int64_t br = 0;
	a2666_bn r = a2666_alloc(a->n);
	for (i = 0; i < a->n; i++) {
		int64_t t = (int64_t)a->d[i] - (i < b->n ? (int64_t)b->d[i] : 0) - br;
		br = 0;
		if (t < 0) { t += 4294967296LL; br = 1; }
		r.d[i] = (uint32_t)t;
	}
	a2666_trim(&r);
	return r;
}

static a2666_bn a2666_add(const a2666_bn *a, const a2666_bn *b)
{
	a2666_bn r;
	if (a->neg == b->neg) {
		r = a2666_addabs(a, b);
		r.neg = a->neg;
	} else if (a2666_cmpabs(a, b) >= 0) {
		r = a2666_subabs(a, b);
		r.neg = a->neg;
	} else {
		r = a2666_subabs(b, a);
		r.neg = b->neg;
	}
	a2666_trim(&r);
	return r;
}

static a2666_bn a2666_sub(const a2666_bn *a, const a2666_bn *b)
{
	a2666_bn nb = *b;
	if (nb.n) nb.neg = !nb.neg;
	return a2666_add(a, &nb);
}

static a2666_bn a2666_mul(const a2666_bn *a, const a2666_bn *b)
{
	size_t i, j;
	a2666_bn r;
	if (a->n == 0 || b->n == 0) return a2666_alloc(0);
	r = a2666_alloc(a->n + b->n);
	for (i = 0; i < a->n; i++) {
		uint64_t c = 0;
		for (j = 0; j < b->n; j++) {
			c += (uint64_t)a->d[i] * b->d[j] + r.d[i + j];
			r.d[i + j] = (uint32_t)c;
			c >>= 32;
		}
		r.d[i + b->n] = (uint32_t)c;
	}
	r.neg = a->neg ^ b->neg;
	a2666_trim(&r);
	return r;
}

/* magnitudes: |a| = q*|b| + r, b nonzero */
static void a2666_divmod(const a2666_bn *a, const a2666_bn *b, a2666_bn *q, a2666_bn *r)
{
	size_t n = b->n, m, i, j;
	int s = 0;
	uint32_t *u, *v;
	if (a2666_cmpabs(a, b) < 0) {
		*q = a2666_alloc(0);
		*r = a2666_copy(a);
		r->neg = 0;
		return;
	}
	if (n == 1) {
		uint64_t rem = 0, dv = b->d[0];
		*q = a2666_alloc(a->n);
		for (i = a->n; i-- > 0;) {
			uint64_t cur = (rem << 32) | a->d[i];
			q->d[i] = (uint32_t)(cur / dv);
			rem = cur % dv;
		}
		a2666_trim(q);
		*r = a2666_u64(rem);
		return;
	}
	m = a->n - n;
	while (!((b->d[n - 1] << s) & 0x80000000u)) s++;
	v = (uint32_t *)calloc(n, sizeof(uint32_t));
	u = (uint32_t *)calloc(a->n + 1, sizeof(uint32_t));
	for (i = n - 1; i > 0; i--)
		v[i] = (b->d[i] << s) | (s ? b->d[i - 1] >> (32 - s) : 0);
	v[0] = b->d[0] << s;
	u[a->n] = s ? a->d[a->n - 1] >> (32 - s) : 0;
	for (i = a->n - 1; i > 0; i--)
		u[i] = (a->d[i] << s) | (s ? a->d[i - 1] >> (32 - s) : 0);
	u[0] = a->d[0] << s;
	*q = a2666_alloc(m + 1);
	for (j = m;; j--) {
		const uint64_t B = 4294967296ULL;
		uint64_t num = ((uint64_t)u[j + n] << 32) | u[j + n - 1];
		uint64_t qhat = num / v[n - 1], rhat = num % v[n - 1];
		int64_t k = 0, t;
		while (qhat >= B || qhat * v[n - 2] > ((rhat << 32) | u[j + n - 2])) {
			qhat--;
			rhat += v[n - 1];
			if (rhat >= B) break;
		}
		for (i = 0; i < n; i++) {
			uint64_t p = qhat * v[i];
			t = (int64_t)u[i + j] - k - (int64_t)(p & 0xFFFFFFFFULL);
			u[i + j] = (uint32_t)t;
			k = (int64_t)(p >> 32) - (t >> 32);
		}
		t = (int64_t)u[j + n] - k;
		u[j + n] = (uint32_t)t;
		q->d[j] = (uint32_t)qhat;
		if (t < 0) {
			uint64_t c = 0;
			q->d[j]--;
			for (i = 0; i < n; i++) {
				c += (uint64_t)u[i + j] + v[i];
				u[i + j] = (uint32_t)c;
				c >>= 32;
			}
			u[j + n] += (uint32_t)c;
		}
		if (j == 0) break;
	}
	*r = a2666_alloc(n);
	for (i = 0; i < n; i++)
		r->d[i] = (u[i] >> s) | (s ? u[i + 1] << (32 - s) : 0);
	a2666_trim(r);
	a2666_trim(q);
	free(u);
	free(v);
}

/* floor(x / y) for y > 0 */
static a2666_bn a2666_floordiv(const a2666_bn *x, const a2666_bn *y)
{
	a2666_bn q, r;
	a2666_divmod(x, y, &q, &r);
	if (x->neg) {
		if (r.n) {
			a2666_bn one = a2666_u64(1), t = a2666_addabs(&q, &one);
			a2666_free(&q);
			a2666_free(&one);
			q = t;
		}
		a2666_negate(&q);
	}
	a2666_free(&r);
	return q;
}

static a2666_bn a2666_exactdiv(const a2666_bn *x, const a2666_bn *y)
{
	a2666_bn q, r;
	a2666_divmod(x, y, &q, &r);
	a2666_free(&r);
	q.neg = x->neg ^ y->neg;
	a2666_trim(&q);
	return q;
}

static void a2666_shr1(a2666_bn *a)
{
	size_t i;
	for (i = 0; i < a->n; i++)
		a->d[i] = (a->d[i] >> 1) | (i + 1 < a->n ? a->d[i + 1] << 31 : 0);
	a2666_trim(a);
}

static a2666_bn a2666_isqrt(const a2666_bn *m)
{
	size_t bits, e;
	uint32_t top;
	a2666_bn x;
	if (m->n == 0) return a2666_alloc(0);
	bits = 32 * (m->n - 1);
	top = m->d[m->n - 1];
	while (top) { bits++; top >>= 1; }
	e = (bits + 1) / 2;
	x = a2666_alloc(e / 32 + 1);
	x.d[e / 32] = 1u << (e % 32);
	for (;;) {
		a2666_bn q, r, y;
		a2666_divmod(m, &x, &q, &r);
		a2666_free(&r);
		y = a2666_addabs(&x, &q);
		a2666_free(&q);
		a2666_shr1(&y);
		if (a2666_cmpabs(&y, &x) >= 0) { a2666_free(&y); break; }
		a2666_free(&x);
		x = y;
	}
	return x;
}

static a2666_bn a2666_gcd(const a2666_bn *a, const a2666_bn *b)
{
	a2666_bn x = a2666_copy(a), y = a2666_copy(b);
	x.neg = 0;
	y.neg = 0;
	while (y.n) {
		a2666_bn q, r;
		a2666_divmod(&x, &y, &q, &r);
		a2666_free(&q);
		a2666_free(&x);
		x = y;
		y = r;
	}
	a2666_free(&y);
	return x;
}

static seq_t a2666_to_seq(const a2666_bn *v)
{
	seq_t r = 0;
	size_t i;
	for (i = v->n; i-- > 0;) {
		r = r * 65536 + (idx_t)(v->d[i] >> 16);
		r = r * 65536 + (idx_t)(v->d[i] & 0xFFFFu);
	}
	return r;
}

seq_t a(idx_t n)
{
	static a2666_bn *memo = NULL;
	static idx_t cnt = 0, cap = 0;
	static a2666_bn P, C, R;
	static int init = 0, big = 0;

	if (!init) {
		seq_t t = 1;
		int i;
		for (i = 0; i < 130; i++) t = t * 2;
		big = (t != 0);
		P = a2666_alloc(0);
		C = a2666_u64(1);
		R = a2666_u64(1);
		init = 1;
	}
	if (!big && n > 8) return 0;

	while (cnt <= n) {
		a2666_bn CC, M2, t, s, b, bR, D, bP, A, t1, t2, nP, bb, one, bb1, CR, nC, DD, nR, g, g2;

		CC = a2666_mul(&C, &C);
		M2 = a2666_add(&CC, &CC);
		t = a2666_isqrt(&M2);
		if (C.neg) {
			a2666_bn tmp;
			one = a2666_u64(1);
			a2666_negate(&t);
			tmp = a2666_sub(&t, &one);
			a2666_free(&t);
			a2666_free(&one);
			t = tmp;
		}
		s = a2666_add(&P, &t);
		b = a2666_floordiv(&s, &R);

		bR = a2666_mul(&b, &R);
		D = a2666_sub(&P, &bR);
		bP = a2666_mul(&b, &P);
		A = a2666_add(&bP, &R);
		t1 = a2666_mul(&A, &D);
		t2 = a2666_mul(&b, &M2);
		nP = a2666_sub(&t1, &t2);
		bb = a2666_mul(&b, &b);
		one = a2666_u64(1);
		bb1 = a2666_add(&bb, &one);
		CR = a2666_mul(&C, &R);
		nC = a2666_mul(&CR, &bb1);
		a2666_negate(&nC);
		DD = a2666_mul(&D, &D);
		nR = a2666_sub(&DD, &M2);
		if (nR.neg) {
			a2666_negate(&nR);
			a2666_negate(&nP);
			a2666_negate(&nC);
		}
		g = a2666_gcd(&nP, &nC);
		g2 = a2666_gcd(&g, &nR);

		a2666_free(&P);
		a2666_free(&C);
		a2666_free(&R);
		P = a2666_exactdiv(&nP, &g2);
		C = a2666_exactdiv(&nC, &g2);
		R = a2666_exactdiv(&nR, &g2);

		a2666_free(&CC); a2666_free(&M2); a2666_free(&t); a2666_free(&s);
		a2666_free(&bR); a2666_free(&D); a2666_free(&bP); a2666_free(&A);
		a2666_free(&t1); a2666_free(&t2); a2666_free(&nP); a2666_free(&bb);
		a2666_free(&one); a2666_free(&bb1); a2666_free(&CR); a2666_free(&nC);
		a2666_free(&DD); a2666_free(&nR); a2666_free(&g); a2666_free(&g2);

		if (cnt == cap) {
			idx_t nc = cap ? cap * 2 : 16;
			memo = (a2666_bn *)realloc(memo, nc * sizeof(a2666_bn));
			cap = nc;
		}
		memo[cnt++] = b;
	}
	return a2666_to_seq(&memo[n]);
}
