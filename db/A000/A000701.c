/*
 * A000701  One half of number of non-self-conjugate partitions; also half of
 *          number of asymmetric Ferrers graphs with n nodes.
 * https://oeis.org/A000701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000041(n) - A000700(n))/2, with p(n) from Euler's
 *            pentagonal recurrence p(n) = Sum_{k>=1} (-1)^(k+1)
 *            (p(n-k(3k-1)/2) + p(n-k(3k+1)/2)), p(0) = 1, carried in two
 *            words so the halving stays exact even when p(n) itself overflows
 *            seq_t; A000700(n) = number of partitions of n into distinct
 *            od...
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 134, u64 n <= 428,
 *            u128 n <= 1479, big any n
 * Helpers:   dw_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/dw_add.h"

static seq_t *Ph = 0, *Pl = 0, *S = 0;
static idx_t cap = 0;

static void build(idx_t N) {
	idx_t i, k, m;
	if (Ph) { free(Ph); free(Pl); free(S); }
	Ph = (seq_t *)calloc(N + 1, sizeof(seq_t));
	Pl = (seq_t *)calloc(N + 1, sizeof(seq_t));
	S = (seq_t *)calloc(N + 1, sizeof(seq_t));
	cap = N + 1;
	Ph[0] = 0;
	Pl[0] = 1;
	for (i = 1; i <= N; i++) {
		seq_t ph = 0, pl = 0, nh = 0, nl = 0;
		for (k = 1; ; k++) {
			idx_t g1 = k * (3 * k - 1) / 2;
			idx_t g2 = k * (3 * k + 1) / 2;
			if (g1 > i) break;
			if (k & 1) {
				dw_add(&ph, &pl, Ph[i - g1], Pl[i - g1]);
				if (g2 <= i) dw_add(&ph, &pl, Ph[i - g2], Pl[i - g2]);
			} else {
				dw_add(&nh, &nl, Ph[i - g1], Pl[i - g1]);
				if (g2 <= i) dw_add(&nh, &nl, Ph[i - g2], Pl[i - g2]);
			}
		}
		/* (ph,pl) - (nh,nl) */
		{
			seq_t borrow = (pl < nl) ? 1 : 0;
			Pl[i] = pl - nl;
			Ph[i] = ph - nh - borrow;
		}
	}
	S[0] = 1;
	for (m = 1; m <= N; m += 2) {
		for (i = N; i >= m; i--) {
			S[i] += S[i - m];
			if (i == m) break;
		}
	}
}

seq_t a(idx_t n) {
	seq_t lo, hi, r;
	if (n + 1 > cap) {
		idx_t N = n;
		if (N < 2 * cap) N = 2 * cap;
		if (N < 64) N = 64;
		build(N);
	}
	lo = Pl[n] - S[n];
	hi = Ph[n] - ((Pl[n] < S[n]) ? 1 : 0);
	r = lo / 2;
	if ((hi & 1) != 0) {
		seq_t mx = (seq_t)0 - 1;
		r += mx - mx / 2;
	}
	return r;
}
