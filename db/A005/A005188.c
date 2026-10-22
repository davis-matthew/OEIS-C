/*
 * A005188  Armstrong (or pluperfect, or Plus Perfect, or narcissistic)
 *          numbers: m-digit nonnegative numbers equal to sum of the m-th
 *          powers of their digits.
 * https://oeis.org/A005188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, nice
 * Formula:   search, not a closed form. For each digit count m, enumerate
 *            multisets of m digits, from digit 9 down to 0; keep a multiset
 *            when the sum of the m-th powers of its digits has m digits and
 *            those digits are exactly that multiset. Branches are cut when
 *            the leading digits that the sum is already bound...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 32, u64 n <= 51, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define NB 100000000000000000ULL /* limb base 10^17 */

typedef struct { uint64_t l[3]; } num_t;

static uint64_t p10[17];
static int M;
static num_t KP[10][41];
static num_t LO, HI;
static int cnt[10];
static num_t found[128];
static int nfound = 0;

static void nadd(num_t *r, const num_t *x, const num_t *y) {
	uint64_t c = 0;
	for (int i = 0; i < 3; i++) {
		uint64_t s = x->l[i] + y->l[i] + c;
		if (s >= NB) { s -= NB; c = 1; } else c = 0;
		r->l[i] = s;
	}
}

static int ncmp(const num_t *x, const num_t *y) {
	for (int i = 2; i >= 0; i--) {
		if (x->l[i] < y->l[i]) return -1;
		if (x->l[i] > y->l[i]) return 1;
	}
	return 0;
}

static int ncmp_v(const void *x, const void *y) {
	return ncmp((const num_t *)x, (const num_t *)y);
}

static int ndigit(const num_t *x, int i) {
	return (int)((x->l[i / 17] / p10[i % 17]) % 10);
}

static void search(int d, int r, num_t S) {
	if (d == 0) {
		int h[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		cnt[0] = r;
		if (ncmp(&S, &LO) < 0 || ncmp(&S, &HI) > 0) return;
		for (int i = 0; i < M; i++) h[ndigit(&S, i)]++;
		for (int j = 0; j < 10; j++)
			if (h[j] != cnt[j]) return;
		if (nfound < 128) found[nfound++] = S;
		return;
	}
	{
		num_t lo = S, hi;
		int need[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int tot = 0;
		nadd(&hi, &S, &KP[d][r]);
		if (ncmp(&lo, &LO) < 0) lo = LO;
		if (ncmp(&hi, &HI) > 0) hi = HI;
		if (ncmp(&lo, &hi) > 0) return;
		for (int i = M - 1; i >= 0; i--) {
			int dl = ndigit(&lo, i), dh = ndigit(&hi, i);
			if (dl != dh) break;
			need[dl]++;
		}
		for (int j = 9; j > d; j--)
			if (need[j] > cnt[j]) return;
		for (int j = 0; j <= d; j++) tot += need[j];
		if (tot > r) return;
	}
	for (int c = 0; c <= r; c++) {
		num_t T;
		nadd(&T, &S, &KP[d][c]);
		if (ncmp(&T, &HI) > 0) break;
		cnt[d] = c;
		search(d - 1, r - c, T);
	}
}

static seq_t to_seq(const num_t *x) {
	int top = 2;
	seq_t v;
	while (top > 0 && x->l[top] == 0) top--;
	v = (seq_t)x->l[top];
	for (int i = top - 1; i >= 0; i--)
		v = v * (seq_t)NB + (seq_t)x->l[i];
	return v;
}

seq_t a(idx_t n) {
	static seq_t *vals = NULL;
	static idx_t nvals = 0;

	if (vals == NULL) {
		p10[0] = 1;
		for (int i = 1; i < 17; i++) p10[i] = p10[i - 1] * 10;
		nfound = 0;
		for (int d = 0; d <= 9; d++) {
			num_t z = {{(uint64_t)d, 0, 0}};
			found[nfound++] = z;
		}
		for (M = 2; M <= 39; M++) {
			num_t zero = {{0, 0, 0}};
			for (int d = 0; d <= 9; d++) {
				num_t p = {{1, 0, 0}};
				for (int e = 0; e < M; e++) {
					uint64_t c = 0;
					for (int i = 0; i < 3; i++) {
						uint64_t s = p.l[i] * (uint64_t)d + c;
						p.l[i] = s % NB;
						c = s / NB;
					}
				}
				KP[d][0] = zero;
				for (int k = 1; k <= M; k++) nadd(&KP[d][k], &KP[d][k - 1], &p);
			}
			LO = zero;
			LO.l[(M - 1) / 17] = p10[(M - 1) % 17];
			HI = zero;
			for (int i = 0; i < M; i++) {
				int li = i / 17;
				HI.l[li] += 9 * p10[i % 17];
			}
			search(9, M, zero);
		}
		qsort(found, (size_t)nfound, sizeof(num_t), ncmp_v);
		vals = (seq_t *)malloc((size_t)nfound * sizeof(seq_t));
		for (int i = 0; i < nfound; i++) vals[i] = to_seq(&found[i]);
		nvals = (idx_t)nfound;
	}

	if (n < 1 || n > nvals) return 0;
	return vals[n - 1];
}
